// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "Item.h"
#include "MotionControllerComponent.h"
#include "HypoxiaCharacter.h"
#include "EngineUtils.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"

AHypoxiaCharacter *HypoxiaCharacter;

// Sets default values
AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	Item_Base = CreateDefaultSubobject<USceneComponent>(TEXT("Item_Base"));

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	MotionController->Hand = EControllerHand::Right;
	MotionController->SetupAttachment(Item_Base);
	MotionController->bAbsoluteLocation = true;
	MotionController->bAbsoluteRotation = true;

	MotionTracker = CreateDefaultSubobject<USceneComponent>(TEXT("MotionTracker"));
	MotionTracker->SetupAttachment(Item_Base);

	Item = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item"));
	Item->SetupAttachment(Item_Base);
	Item->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void AItem::BeginPlay() {

	Super::BeginPlay();

	Held = false;

	Item->SetSimulatePhysics(true);
	Item->SetEnableGravity(true);

	for (TActorIterator<AHypoxiaCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		//This may need adjusting to ensure it gets the right one
		HypoxiaCharacter = *ActorItr;
	}

}

bool AItem::Pickup(USceneComponent* Controller, EControllerHand Hand) {
	//UE_LOG(LogTemp, Warning, TEXT("Dist %f"), FVector::Dist(Controller->GetComponentLocation(), Item_Base->GetComponentLocation()));

	if (!Held) {

		//If the item is within the given distance of the given controller, pick it up
		if (FVector::Dist(Controller->GetComponentLocation(), Item->GetComponentLocation()) < 250.0f) {

			//Item->SetSimulatePhysics(false);
			Item->SetEnableGravity(false);
			Item->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, 0.0f));
			Item->SetPhysicsAngularVelocity(FVector(0.0f, 0.0f, 0.0f));

			//Attach the components, don't move the item if something fails with that
			if (Item_Base->AttachToComponent(HypoxiaCharacter->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale)) {
				HypoxiaCharacter->SetHeldItem(this, Hand);
				Item->AttachToComponent(MotionTracker, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				MotionController->Hand = Hand;
				Held = true;
				return true;
				//UE_LOG(LogTemp, Warning, TEXT("It worked :)"));
			} else {
				//Item->SetSimulatePhysics(true);
			}
		}
	}

	return false;

}

void AItem::Drop() {

	if (Held) {
		//Item->SetSimulatePhysics(true);
		//Item->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, 0.0f));
		Item->SetEnableGravity(true);
		//Item->SetPhysicsLinearVelocity(Item->GetPhysicsLinearVelocity() * FVector(20.0f, 20.0f, 2.0f));
		Item_Base->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		Item->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		Held = false;
	}
}

void AItem::Use() {}

void AItem::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);

	if (Held) {
		FVector  DevicePosition;
		FRotator DeviceRotation;

		UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(DeviceRotation, DevicePosition);

		MotionTracker->SetWorldLocation(MotionController->GetComponentLocation() + RootComponent->GetComponentLocation() - DevicePosition + FVector(0.0f, 0.0f, DevicePosition.Z - 105.f), false, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);
		MotionTracker->SetWorldRotation(MotionController->GetComponentRotation() + RootComponent->GetComponentRotation() - FRotator(0.0f, DeviceRotation.Yaw, 0.0f));

		Item->SetWorldLocation(MotionTracker->GetComponentLocation(), true, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);
		Item->SetWorldRotation(MotionTracker->GetComponentRotation(), true, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);

		//false, (FHitResult*)nullptr, ETeleportType::TeleportPhysics

		if (FVector::Dist(MotionTracker->GetComponentLocation(), Item->GetComponentLocation()) > 250.0f) {
			Drop();
		}
	}

}