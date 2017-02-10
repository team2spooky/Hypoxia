// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "ComplexAudioComponent.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationPath.h"

UComplexAudioComponent::UComplexAudioComponent() : Super() {
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USoundAttenuation> AttenuationAsset((TEXT("/Game/TestAttenuation.TestAttenuation")));

	VirtualAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("VirtualAudioComponent"));
	VirtualAudioComponent->bAutoActivate = true;
	VirtualAudioComponent->AttenuationSettings = AttenuationAsset.Object;
	VirtualAudioComponent->SetWorldLocation(this->GetComponentLocation());

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
	this->Play();
}

void UComplexAudioComponent::TickComponent(float deltaSeconds, ELevelTick type, FActorComponentTickFunction* tickFunction) {
	FVector Loc = this->GetComponentLocation();
	float Vol = 0.0f;
	if (TestOcclusion()) {
		DiffractSound(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation(), Loc, Vol);
#if !UE_BUILD_SHIPPING	
		::DrawDebugLine(GetWorld(), VirtualAudioComponent->GetComponentLocation(), UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation(), FColor::Red);
		::DrawDebugLine(GetWorld(), this->GetComponentLocation(), VirtualAudioComponent->GetComponentLocation(), FColor::Red);
#endif
	}
	VirtualAudioComponent->SetWorldLocation(Loc);
	VirtualAudioComponent->SetVolumeMultiplier(Vol);
}

void UComplexAudioComponent::Play(float startTime) {
	Super::Play(startTime);
	VirtualAudioComponent->Play(startTime);
}

bool UComplexAudioComponent::TestOcclusion() {
	ACharacter* PC = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	bool Obstructed = GetWorld()->LineTraceTestByChannel(GetComponentLocation(), PC->GetActorLocation(), ECC_Visibility);
#if !UE_BUILD_SHIPPING	
	::DrawDebugLine(GetWorld(), this->GetComponentLocation(), PC->GetActorLocation(), FColor::Red);
#endif
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
	out_Vol = FMath::Max(0.9f - ((Path->GetLength() - Projection.Size()) / 2000.0f), 0.0f);
	//out_Vol = 1.0f;
	//UE_LOG(LogTemp, Warning, TEXT("Volume = %f"), Out_Vol);
}
