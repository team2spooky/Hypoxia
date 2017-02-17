// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "ComplexAudioComponent.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationPath.h"
#include "ListeningItem.h"

UComplexAudioComponent::UComplexAudioComponent() : Super() {
	PrimaryComponentTick.bCanEverTick = true;

	//static ConstructorHelpers::FObjectFinder<USoundAttenuation> AttenuationAsset((TEXT("/Game/TestAttenuation.TestAttenuation")));

	VirtualAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("VirtualAudioComponent"));
	VirtualAudioComponent->bAutoActivate = true;
	//VirtualAudioComponent->AttenuationSettings = AttenuationAsset.Object; // Old way of applying attenuation
	//VirtualAudioComponent->AttenuationSettings = this->AttenuationSettings; // Does not work
	VirtualAudioComponent->SetWorldLocation(this->GetComponentLocation());

	InfluenceSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Influence Sphere"));
	InfluenceSphere->SetupAttachment(this);

	// Debug Sphere
	UStaticMeshComponent* VirtualDebugSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DEBUGVIRTUAL"));
	VirtualDebugSphere->SetupAttachment(VirtualAudioComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/Startercontent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded()) {
		VirtualDebugSphere->SetStaticMesh(SphereVisualAsset.Object);
		VirtualDebugSphere->SetWorldScale3D(FVector(0.1f));
		VirtualDebugSphere->SetCollisionProfileName(TEXT("NoCollision"));
	}
}

void UComplexAudioComponent::BeginPlay() {
	VirtualAudioComponent->SetSound(this->Sound);
	//this->Stop();
	
	// Copy Attenuation settings
	VirtualAudioComponent->AttenuationSettings = this->AttenuationSettings;
	//VirtualAudioComponent->AdjustAttenuation(*this->GetAttenuationSettingsToApply());

	// Setup Influence Sphere
	InfluenceSphere->SetSphereRadius(this->GetAttenuationSettingsToApply()->GetMaxDimension());

	this->Play();
}

void UComplexAudioComponent::TickComponent(float deltaSeconds, ELevelTick type, FActorComponentTickFunction* tickFunction) {
	FVector Loc = this->GetComponentLocation();
	float Vol = 0.0f;
	float Occlusion = TestOcclusion();
	if (Occlusion) {
		DiffractSound(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation(), Loc, Vol);
#if !UE_BUILD_SHIPPING	
		::DrawDebugLine(GetWorld(), VirtualAudioComponent->GetComponentLocation(), UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation(), FColor::Red, false, -1.0f, (uint8)'\000', 0.1f);
		::DrawDebugLine(GetWorld(), this->GetComponentLocation(), VirtualAudioComponent->GetComponentLocation(), FColor::Red, false, -1.0f, (uint8)'\000', 0.1f);
#endif
	}
	this->SetVolumeMultiplier(1 - Occlusion);
	VirtualAudioComponent->SetWorldLocation(Loc);
	VirtualAudioComponent->SetVolumeMultiplier(Vol * Occlusion);
	if (this->IsPlaying()) {
		//USoundWave* temp = Cast<USoundWave>(this->Sound);
		//Cast<USoundCue>(this->Sound);
		//if(temp != nullptr)
		//UE_LOG(LogTemp, Warning, TEXT("Volume = %f"), this->GetAttenuationSettingsToApply()->GetMaxDimension());
		TSet<AActor*> OverlappingActors;
		TSubclassOf<AListeningItem> Filter = AListeningItem::StaticClass();
		float MaxDist = this->GetAttenuationSettingsToApply()->GetMaxDimension();
		InfluenceSphere->GetOverlappingActors(OverlappingActors, Filter);
		for (TSet<AActor*>::TConstIterator Itr = OverlappingActors.CreateConstIterator(); Itr; ++Itr) {
			AListeningItem* Item = Cast<AListeningItem>(*Itr);
			float Dist = FVector::Dist(Item->GetActorLocation(), this->GetComponentLocation());
			Item->Hear(FMath::Lerp(100.f, 0.f, Dist / MaxDist) - 50.f);
		}
	}
}

void UComplexAudioComponent::Play(float startTime) {
	Super::Play(startTime);
	VirtualAudioComponent->Play(startTime);
}

void UComplexAudioComponent::Stop() {
	Super::Stop();
	VirtualAudioComponent->Stop();
}

float UComplexAudioComponent::TestOcclusion() {
	ACharacter* PC = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	float Obstructed = 0.0f;
	if (!bAdvancedOcclusion) {
		Obstructed = GetWorld()->LineTraceTestByChannel(GetComponentLocation(), PC->GetActorLocation(), ECC_Visibility);
#if !UE_BUILD_SHIPPING	
		::DrawDebugLine(GetWorld(), this->GetComponentLocation(), PC->GetActorLocation(), FColor::Red, false, -1.0f, (uint8)'\000', 0.1f);
#endif
	} else {
		FVector RVec = PC->GetActorUpVector() * Radius;
		FVector Axis = PC->GetActorLocation() - GetComponentLocation();
		Axis.Normalize();
		for (int i = 0; i < 8; i++) {
			FVector TestLoc = GetComponentLocation() + RVec.RotateAngleAxis(i * 360.0f / 8, Axis);
			Obstructed += GetWorld()->LineTraceTestByChannel(TestLoc, PC->GetActorLocation(), ECC_Visibility);
#if !UE_BUILD_SHIPPING	
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
	out_Vol = FMath::Clamp(1.0f - ((Path->GetLength() - Projection.Size()) / 2000.0f), 0.0f, 1.0f);
	//out_Vol = 1.0f;
	UE_LOG(LogTemp, Warning, TEXT("Volume = %f"), out_Vol);
}
