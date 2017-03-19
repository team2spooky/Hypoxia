// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "HypoxiaMonster.h"
#include "HypoxiaAIController.h"
#include "ComplexAudioComponent.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationPath.h"
#include "ListeningItem.h"

#define AUDIO_DEBUG 0

UComplexAudioComponent::UComplexAudioComponent() : Super() {
	PrimaryComponentTick.bCanEverTick = true;

	//static ConstructorHelpers::FObjectFinder<USoundAttenuation> AttenuationAsset((TEXT("/Game/TestAttenuation.TestAttenuation")));

	VirtualAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("VirtualAudioComponent"));
	VirtualAudioComponent->bAutoActivate = true;
	//VirtualAudioComponent->AttenuationSettings = AttenuationAsset.Object; // Old way of applying attenuation
	//VirtualAudioComponent->AttenuationSettings = this->AttenuationSettings; // Does not work
	VirtualAudioComponent->SetWorldLocation(this->GetComponentLocation());
	VirtualAudioComponent->RegisterComponent();

	InfluenceSphereAudio = CreateDefaultSubobject<USphereComponent>(TEXT("InfluenceSphereAudio"));
	InfluenceSphereAudio->bAutoActivate = true;
	InfluenceSphereAudio->SetWorldLocation(this->GetComponentLocation());
	//InfluenceSphereAudio->SetupAttachment(GetAttachParent());
	InfluenceSphereAudio->RegisterComponent();

#if AUDIO_DEBUG
	InfluenceSphereAudio->bHiddenInGame = false;
	// Debug Sphere
	UStaticMeshComponent* VirtualDebugSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DEBUGVIRTUAL"));
	VirtualDebugSphere->SetupAttachment(VirtualAudioComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/Startercontent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded()) {
		VirtualDebugSphere->SetStaticMesh(SphereVisualAsset.Object);
		VirtualDebugSphere->SetWorldScale3D(FVector(0.1f));
		VirtualDebugSphere->SetCollisionProfileName(TEXT("NoCollision"));
	}
#endif
}

void UComplexAudioComponent::BeginPlay() {
	if(this->Sound)
		SetSound(this->Sound);
	bool AutoDestroy = this->bAutoDestroy;
	this->bAutoDestroy = false;
	this->Stop();

	InfluenceSphereAudio->SetWorldLocation(GetComponentLocation());
	InfluenceSphereAudio->AttachToComponent(this, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));

	// Copy Attenuation settings
	if (this->AttenuationSettings != nullptr) {
		//VirtualAudioComponent->AttenuationSettings = this->AttenuationSettings;
		VirtualAudioComponent->bOverrideAttenuation = true;
		VirtualAudioComponent->AttenuationOverrides = this->AttenuationSettings->Attenuation;

		// Setup Influence Sphere
		InfluenceSphereAudio->SetSphereRadius(this->GetAttenuationSettingsToApply()->GetMaxDimension() * ProjectedVolume / 100.f);
	}

	this->bAutoDestroy = AutoDestroy;
	this->Play();

	//UE_LOG(LogTemp, Warning, TEXT("%d"), InfluenceSphereAudio->IsWelded());
}

void UComplexAudioComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector Loc = this->GetComponentLocation();
	float Vol = 0.0f;
	float Occlusion = 0.f;
	if (FVector::Dist(Loc, UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation()) <= this->GetAttenuationSettingsToApply()->FalloffDistance) {
		Occlusion = TestOcclusion();
		if (Occlusion) {
			DiffractSound(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation(), Loc, Vol);
#if AUDIO_DEBUG	
			::DrawDebugLine(GetWorld(), VirtualAudioComponent->GetComponentLocation(), UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation(), FColor::Red, false, -1.0f, (uint8)'\000', 0.1f);
			::DrawDebugLine(GetWorld(), this->GetComponentLocation(), VirtualAudioComponent->GetComponentLocation(), FColor::Red, false, -1.0f, (uint8)'\000', 0.1f);
#endif
		}
	}
	this->SetVolumeMultiplier(1 - Occlusion);
	VirtualAudioComponent->SetWorldLocation(Loc);
	VirtualAudioComponent->SetVolumeMultiplier(Vol * Occlusion);
	
	if (this->IsPlaying() && !bAmbientSound) {
		//USoundWave* temp = Cast<USoundWave>(this->Sound);
		//Cast<USoundCue>(this->Sound);
		//if(temp != nullptr)
		//UE_LOG(LogTemp, Warning, TEXT("Volume = %f"), this->GetAttenuationSettingsToApply()->GetMaxDimension());
		TSet<AActor*> OverlappingActors;
		TSubclassOf<AListeningItem> Filter = AListeningItem::StaticClass();
		float MaxDist = this->GetAttenuationSettingsToApply()->GetMaxDimension();
		InfluenceSphereAudio->GetOverlappingActors(OverlappingActors, Filter);
		if (!bListenToSelf) {
			OverlappingActors.Remove(this->GetAttachmentRootActor());
		}
		for (TSet<AActor*>::TConstIterator Itr = OverlappingActors.CreateConstIterator(); Itr; ++Itr) {
			AListeningItem* Item = Cast<AListeningItem>(*Itr);
			if (GetWorld()->LineTraceTestByChannel(this->GetComponentLocation(), Item->GetItem()->GetComponentLocation(), ECC_GameTraceChannel2))
				continue;
			float Dist = FVector::Dist(Item->GetItem()->GetComponentLocation(), this->GetComponentLocation());
			Item->Hear(FMath::Lerp(1.f, 0.f, Dist / MaxDist) * ProjectedVolume);
		}
		TSubclassOf<AHypoxiaMonster> Monster = AHypoxiaMonster::StaticClass();
		InfluenceSphereAudio->GetOverlappingActors(OverlappingActors, Monster);
		if (!bListenToSelf) {
			OverlappingActors.Remove(this->GetAttachmentRootActor());
		}
		for (TSet<AActor*>::TConstIterator Itr = OverlappingActors.CreateConstIterator(); Itr; ++Itr) {
			AHypoxiaMonster* M = Cast<AHypoxiaMonster>(*Itr);
			/*if (GetWorld()->LineTraceTestByChannel(this->GetComponentLocation(), M->GetActorLocation(), ECC_GameTraceChannel2))
				continue;*/
			Cast<AHypoxiaAIController>(M->GetController())->HearSound(InfluenceSphereAudio->GetComponentLocation(), ProjectedVolume);
		}
	}
}

void UComplexAudioComponent::DestroyComponent(bool bPromoteChildren) {
	VirtualAudioComponent->DestroyComponent(false);
	InfluenceSphereAudio->DestroyComponent(false);
	Super::DestroyComponent(bPromoteChildren);
}

void UComplexAudioComponent::Play(float startTime) {
	Super::Play(startTime);
	VirtualAudioComponent->Play(startTime);
}

void UComplexAudioComponent::Stop() {
	Super::Stop();
	VirtualAudioComponent->Stop();
}

void UComplexAudioComponent::SetSound(USoundBase* newSound) {
	VirtualAudioComponent->SetSound(newSound);
	Super::SetSound(newSound);
}

void UComplexAudioComponent::SetAttenuationSettings(USoundAttenuation* newAttenuation) {
	this->AttenuationSettings = newAttenuation;
	VirtualAudioComponent->AttenuationSettings = this->AttenuationSettings;

	// Setup Influence Sphere
	InfluenceSphereAudio->SetSphereRadius(this->GetAttenuationSettingsToApply()->GetMaxDimension());
}

float UComplexAudioComponent::TestOcclusion() {
	ACharacter* PC = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	float Obstructed = 0.0f;
	if (!bAdvancedOcclusion) {
		Obstructed = GetWorld()->LineTraceTestByChannel(GetComponentLocation(), PC->GetActorLocation(), ECC_GameTraceChannel2);
#if AUDIO_DEBUG	
		::DrawDebugLine(GetWorld(), this->GetComponentLocation(), PC->GetActorLocation(), FColor::Red, false, -1.0f, (uint8)'\000', 0.1f);
#endif
	} else {
		FVector RVec = PC->GetActorUpVector() * Radius;
		FVector Axis = PC->GetActorLocation() - GetComponentLocation();
		Axis.Normalize();
		for (int i = 0; i < 8; i++) {
			FVector TestLoc = GetComponentLocation() + RVec.RotateAngleAxis(i * 360.0f / 8, Axis);
			Obstructed += GetWorld()->LineTraceTestByChannel(TestLoc, PC->GetActorLocation(), ECC_GameTraceChannel2);
#if AUDIO_DEBUG
			::DrawDebugLine(GetWorld(), TestLoc, PC->GetActorLocation(), FColor::Green, false, -1.0f, (uint8)'\000', 0.1f);
#endif
		}
		Obstructed /= 8;
	}
	return Obstructed;
}

void UComplexAudioComponent::DiffractSound(FVector goalLoc, FVector& out_Loc, float& out_Vol) {
	UNavigationSystem* NavSys = UNavigationSystem::GetCurrent(GetWorld());
	FNavPathSharedPtr Path = NavSys->FindPathToLocationSynchronously(GetWorld(), this->GetComponentLocation(), goalLoc)->GetPath();
	if (!Path.IsValid())
		return;
	TArray<FNavPathPoint> PathPoints = Path->GetPathPoints();
	if (PathPoints.Num() <= 2) {
		return;
	}
	FVector Target = PathPoints[PathPoints.Num() - 2].Location;
	Target.Z = goalLoc.Z;
	FVector Projection = Target - goalLoc;
	out_Loc = Target;
	out_Vol = FMath::Clamp(1.0f - ((Path->GetLength() - Projection.Size()) / this->AttenuationSettings->Attenuation.FalloffDistance), 0.0f, 1.0f);
	FAttenuationSettings AdjustedSettings = FAttenuationSettings(*VirtualAudioComponent->GetAttenuationSettingsToApply());
	AdjustedSettings.FalloffDistance = this->AttenuationSettings->Attenuation.FalloffDistance - (Path->GetLength() - Projection.Size());
	VirtualAudioComponent->AdjustAttenuation(AdjustedSettings);
	//out_Vol = 1.0f;
#if AUDIO_DEBUG	
	UE_LOG(LogTemp, Warning, TEXT("Volume = %f, Loc = (%f, %f, %f)"), out_Vol, out_Loc.X, out_Loc.Y, out_Loc.Z);
#endif
}
