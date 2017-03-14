// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "LuminousPlant.h"

ALuminousPlant::ALuminousPlant() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObject(TEXT("/Game/Hypoxia/Meshes/luminous_plant.luminous_plant"));
	if (MeshObject.Succeeded()) {
		Item->SetStaticMesh(MeshObject.Object);
	}

	Particles = CreateDefaultSubobject<UParticleSystemComponent>(FName("GlowParticles"));
	//Particles->SetAutoAttachmentParameters(Item, NAME_None, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld);
	Particles->SetupAttachment(Item);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystem(TEXT("/Game/Hypoxia/Effects/P_Luminous_Plant.P_Luminous_Plant"));
	if(ParticleSystem.Succeeded())
		Particles->SetTemplate(ParticleSystem.Object);
}

void ALuminousPlant::BeginPlay() {
	Super::BeginPlay();
	DynamicMaterial = Item->CreateAndSetMaterialInstanceDynamic(0);
	Particles->CreateAndSetMaterialInstanceDynamic(0);
	Particles->AutoPopulateInstanceProperties();
}

void ALuminousPlant::Tick(float deltaSeconds) {
	float Glow;
	DynamicMaterial->GetScalarParameterValue(FName("GlowIntensity"), Glow);
	DynamicMaterial->SetScalarParameterValue(FName("GlowIntensity"), FMath::Max(Glow - 50 * deltaSeconds, 0.f));
	Particles->SetFloatParameter("SpawnRate", Glow / 5);
}

void ALuminousPlant::Hear(float volume) {
	float CurrentGlow;
	DynamicMaterial->GetScalarParameterValue(FName("GlowIntensity"), CurrentGlow);
	float NewGlow = FMath::Max(CurrentGlow, volume * 2);
	DynamicMaterial->SetScalarParameterValue(FName("GlowIntensity"), NewGlow);
	Particles->ActivateSystem();
}


