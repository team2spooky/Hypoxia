// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "PlantConductor.h"
#include "PlantSocket.h"

APlantConductor::APlantConductor() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObject(TEXT("/Game/Hypoxia/Meshes/luminous_plant.luminous_plant"));
	if (MeshObject.Succeeded()) {
		Item->SetStaticMesh(MeshObject.Object);
	}
	Item->SetCollisionProfileName(TEXT("Item"));

	Particles = CreateDefaultSubobject<UParticleSystemComponent>(FName("GlowParticles"));
	//Particles->SetAutoAttachmentParameters(Item, NAME_None, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld);
	Particles->SetupAttachment(Item);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystem(TEXT("/Game/Hypoxia/Effects/P_Luminous_Plant.P_Luminous_Plant"));
	if (ParticleSystem.Succeeded())
		Particles->SetTemplate(ParticleSystem.Object);

	GlowLight = CreateDefaultSubobject<UPointLightComponent>(FName("GlowLight"));
	GlowLight->SetupAttachment(Item);
	GlowLight->SetLightColor(FColor(141, 0, 255));
	GlowLight->SetIntensity(0.f);
}

void APlantConductor::BeginPlay() {
	Super::BeginPlay();
	Particles->DetachFromParent();
	Particles->AttachToComponent(Item, FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true));
	DynamicMaterial = Item->CreateAndSetMaterialInstanceDynamic(0);
	Particles->CreateAndSetMaterialInstanceDynamic(0);
	Particles->AutoPopulateInstanceProperties();
}

void APlantConductor::Tick(float deltaSeconds) {
	Super::Tick(deltaSeconds);
	GlowIntensity = FMath::Max(GlowIntensity - 50 * deltaSeconds, 1.f);
	DynamicMaterial->SetScalarParameterValue(FName("GlowIntensity"), GlowIntensity);
	Particles->SetFloatParameter("SpawnRate", FMath::Max(GlowIntensity / 7.5f, 0.1f));
	GlowLight->SetIntensity(GlowIntensity);
}

void APlantConductor::Drop() {
	Super::Drop();
	TArray<AActor*> Sockets;
	UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), APlantSocket::StaticClass(), Sockets);
	for (AActor* Socket : Sockets) {
		if (FVector::Dist(Socket->GetActorLocation(), Item->GetComponentLocation()) < 50) {
			Cast<APlantSocket>(Socket)->Conductor = this;
			return;
		}
	}
}

void APlantConductor::Hear(float volume) {
	GlowIntensity = FMath::Max(GlowIntensity, volume * 1.5f);
	Particles->ActivateSystem();
}


