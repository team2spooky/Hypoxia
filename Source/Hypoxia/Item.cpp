// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "Item.h"
#include "MotionControllerComponent.h"
#include "HypoxiaCharacter.h"
#include "EngineUtils.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"

AHypoxiaCharacter *HypoxiaCharacter;

bool Held;

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

	Item = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Item"));
	Item->SetupAttachment(Item_Base);
	Item->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void AItem::BeginPlay() {

	Super::BeginPlay();

	Held = false;

	Item->SetSimulatePhysics(true);

	for (TActorIterator<AHypoxiaCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		//This may need adjusting to ensure it gets the right one
		HypoxiaCharacter = *ActorItr;
	}

}

void AItem::Pickup(USceneComponent* Controller, EControllerHand Hand) {
	//UE_LOG(LogTemp, Warning, TEXT("Dist %f"), FVector::Dist(Controller->GetComponentLocation(), Item_Base->GetComponentLocation()));

	if (!Held) {

		//If the item is within the given distance of the given controller, pick it up
		if (FVector::Dist(Controller->GetComponentLocation(), Item->GetComponentLocation()) < 350.0f) {

			Item->SetSimulatePhysics(false);
			//Item->SetEnableGravity(false);

			//Attach the components, don't move the item if something fails with that
			if (Item_Base->AttachToComponent(HypoxiaCharacter->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale)) {
				HypoxiaCharacter->SetHeldItem(this, Hand);
				Item->AttachToComponent(MotionTracker, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				MotionController->Hand = Hand;
				Held = true;
				//UE_LOG(LogTemp, Warning, TEXT("It worked :)"));
			} else {
				Item->SetSimulatePhysics(true);
			}
		}
	}
}

void AItem::Drop() {

	if (Held) {
		Item->SetSimulatePhysics(true);
		//Item->SetEnableGravity(true);
		//Item->SetPhysicsLinearVelocity(Item->GetPhysicsLinearVelocity() * 2);
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

		//HMDPositionDelta = DevicePosition - LastHMDPosition;
		//LastHMDPosition = DevicePosition;

		//AddMovementInput(HMDPositionDelta, MOVEMENT_SCALE);

		//FirstPersonCameraComponent->SetWorldRotation(DeviceRotation);

		//RootComponent->SetWorldLocation(DevicePosition);

		//R_MotionController->SetWorldLocation(RootComponent->GetComponentLocation());

		//UE_LOG(LogTemp, Error, TEXT("Position  X: %f"), RootComponent->GetComponentLocation().X);

		/*UE_LOG(LogTemp, Error, TEXT("Component X: %f"), R_MotionController->GetComponentLocation().X);
		UE_LOG(LogTemp, Error, TEXT("Component Y: %f"), R_MotionController->GetComponentLocation().Y);
		UE_LOG(LogTemp, Error, TEXT("Component Z: %f"), R_MotionController->GetComponentLocation().Z);*/

		MotionTracker->SetWorldLocation(MotionController->GetComponentLocation() + RootComponent->GetComponentLocation() - DevicePosition + FVector(0.0f, 0.0f, 60.0f));
		MotionTracker->SetWorldRotation(MotionController->GetComponentRotation() + RootComponent->GetComponentRotation() - FRotator(0.0f, DeviceRotation.Yaw, 0.0f));
	}

}