// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "PlantConductor.h"

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
	Particles->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	Particles->AttachToComponent(Item, FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true));
	DynamicMaterial = Item->CreateAndSetMaterialInstanceDynamic(0);
	Particles->CreateAndSetMaterialInstanceDynamic(0);
	Particles->AutoPopulateInstanceProperties();
	Drop();
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
	for (AActor* S : Sockets) {
		APlantSocket* socket = Cast<APlantSocket>(S);
		if (FVector::Dist(socket->GetActorLocation(), Item->GetComponentLocation()) < socket->SnapDistance) {
			FRotator RotationDiff = socket->GetActorRotation() - Item->GetComponentRotation();
			if (FMath::Abs(RotationDiff.Pitch) < socket->AngleTolerance && FMath::Abs(RotationDiff.Roll) < socket->AngleTolerance) {
				Item->SetSimulatePhysics(false);
				Item->SetWorldLocation(socket->GetActorLocation());
				Item->SetWorldRotation(socket->GetActorRotation());
				this->Socket = socket;
				return;
			}
		}
	}
}

bool APlantConductor::Pickup(USceneComponent* SceneComponent, EControllerHand ControllerHand) {
	bool returnValue = Super::Pickup(SceneComponent, ControllerHand);
	if (returnValue) {
		Item->SetSimulatePhysics(true);
		this->Socket = NULL;
	}
	return returnValue;
}

void APlantConductor::Hear(float volume) {
	GlowIntensity = FMath::Max(GlowIntensity, volume * 1.5f);
	Particles->ActivateSystem();
	if (IsValid(this->Socket)) {
		this->Socket->Power(volume);
	}
}


