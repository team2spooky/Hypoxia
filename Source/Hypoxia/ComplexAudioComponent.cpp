// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "HypoxiaSound.h"
#include "ComplexAudioComponent.h"

UAudioComponent* VirtualAudioComponent;

UComplexAudioComponent::UComplexAudioComponent() : Super() {
	PrimaryComponentTick.bCanEverTick = true;
	this->bAlwaysPlay = true;

	static ConstructorHelpers::FObjectFinder<USoundBase> SoundAsset((TEXT("/Game/StarterContent/Audio/Starter_Music_Cue.Starter_Music_Cue")));
	static ConstructorHelpers::FObjectFinder<USoundAttenuation> AttenuationAsset((TEXT("/Game/TestAttenuation.TestAttenuation")));

	VirtualAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("VirtualAudioComponent"));
	VirtualAudioComponent->bAutoActivate = true;
	VirtualAudioComponent->bAlwaysPlay = true;
	VirtualAudioComponent->SetSound(SoundAsset.Object);
	VirtualAudioComponent->AttenuationSettings = AttenuationAsset.Object;
	//VirtualAudioComponent->SetSound(SoundAsset.Object);
	//VirtualAudioComponent->AttenuationSettings = AttenuationAsset.Object;
	//VirtualAudioComponent->SetupAttachment(this);
	VirtualAudioComponent->SetWorldLocation(this->GetComponentLocation());
	//VirtualAudioComponent->RegisterComponent();

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
}

void UComplexAudioComponent::TickComponent(float deltaSeconds, ELevelTick type, FActorComponentTickFunction* tickFunction) {
	//if(!VirtualAudioComponent->IsPlaying())
		//VirtualAudioComponent->Play();
	if (FVector::Dist(this->GetComponentLocation(), UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation()) > 6000.0f) return;
	FVector EndLoc;
	float Vol;
	UHypoxiaSound::PathSoundTo(GetWorld(), this->GetComponentLocation(), UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation(), EndLoc, Vol);
	VirtualAudioComponent->SetWorldLocation(EndLoc);
	VirtualAudioComponent->SetVolumeMultiplier(Vol);
}
