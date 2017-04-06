// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "LuminousPlant.h"

ALuminousPlant::ALuminousPlant() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObject(TEXT("/Game/Hypoxia/Meshes/luminous_plant.luminous_plant"));
	if (MeshObject.Succeeded()) {
		Item->SetStaticMesh(MeshObject.Object);
	}
	Item->SetCollisionProfileName(TEXT("Item"));

	Particles = CreateDefaultSubobject<UParticleSystemComponent>(FName("GlowParticles"));
	//Particles->SetAutoAttachmentParameters(Item, NAME_None, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld);
	Particles->SetupAttachment(Item);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystem(TEXT("/Game/Hypoxia/Effects/P_Luminous_Plant.P_Luminous_Plant"));
	if(ParticleSystem.Succeeded())
		Particles->SetTemplate(ParticleSystem.Object);

	GlowLight = CreateDefaultSubobject<UPointLightComponent>(FName("GlowLight"));
	GlowLight->SetupAttachment(Item);
	GlowLight->SetLightColor(FColor(141, 0, 255));
	GlowLight->SetIntensity(0.f);
}

void ALuminousPlant::BeginPlay() {
	Super::BeginPlay();
	Particles->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	Particles->AttachToComponent(Item, FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true));
	DynamicMaterial = Item->CreateAndSetMaterialInstanceDynamic(0);
	Particles->CreateAndSetMaterialInstanceDynamic(0);
	Particles->AutoPopulateInstanceProperties();
}

void ALuminousPlant::Tick(float deltaSeconds) {
	Super::Tick(deltaSeconds);
	float Glow;
	DynamicMaterial->GetScalarParameterValue(FName("GlowIntensity"), Glow);
	DynamicMaterial->SetScalarParameterValue(FName("GlowIntensity"), FMath::Max(Glow - 50 * deltaSeconds, 1.f));
	Particles->SetFloatParameter("SpawnRate", FMath::Max(Glow / 7.5f, 0.1f));
	GlowLight->SetIntensity(Glow);
}

void ALuminousPlant::Hear(float volume) {
	float CurrentGlow;
	DynamicMaterial->GetScalarParameterValue(FName("GlowIntensity"), CurrentGlow);
	float NewGlow = FMath::Max(CurrentGlow, volume * 1.5f);
	DynamicMaterial->SetScalarParameterValue(FName("GlowIntensity"), NewGlow);
	Particles->ActivateSystem();
}


