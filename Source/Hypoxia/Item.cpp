// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "Item.h"
#include "MotionControllerComponent.h"
#include "HypoxiaCharacter.h"
#include "EngineUtils.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"

AHypoxiaCharacter *HypoxiaCharacter;

const float ARM_STRENGTH = 400.0f; //Applifies the velocity of objects when dropped

FVector OldLocation;
FVector NewLocation;

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

	Item->GetBodyInstance()->bLockTranslation = false;
	Item->GetBodyInstance()->bLockRotation    = false;
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
		Held = false;
		//Item->SetSimulatePhysics(true);
		Item->SetEnableGravity(true);
		Item->SetPhysicsLinearVelocity(MotionController->GetPhysicsLinearVelocity());
		Item_Base->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		Item->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		Item->SetPhysicsLinearVelocity(FVector(((NewLocation - OldLocation) * ARM_STRENGTH).X, ((NewLocation - OldLocation) * ARM_STRENGTH).Y, ((NewLocation - OldLocation) * ARM_STRENGTH).Z));
	}
}

void AItem::Use() {}

void AItem::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);

	if (Held) {
		FVector  DevicePosition;
		FRotator DeviceRotation;

		UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(DeviceRotation, DevicePosition);

		MotionTracker->SetWorldLocation(MotionController->GetComponentLocation() + RootComponent->GetComponentLocation() - DevicePosition + FVector(0.0f, 0.0f, DevicePosition.Z - 105.f), false, (FHitResult*)nullptr, ETeleportType::None);
		MotionTracker->SetWorldRotation(MotionController->GetComponentRotation() + RootComponent->GetComponentRotation() - FRotator(0.0f, DeviceRotation.Yaw, 0.0f));

		OldLocation = NewLocation;
		NewLocation = MotionTracker->GetComponentLocation();

		//UE_LOG(LogTemp, Warning, TEXT("Velox: %f"), ((NewLocation - OldLocation) * 10.0f).X);

		if (!Item->GetBodyInstance()->bLockTranslation) {
			Item->SetWorldLocation(MotionTracker->GetComponentLocation(), true, (FHitResult*)nullptr, ETeleportType::None);
		}

		if (!Item->GetBodyInstance()->bLockRotation) {
			FRotator NewRotator = FRotator(0.0f, 0.0f, 0.0f);

			//If someone's bored, make these use the ? operator
			if (!Item->GetBodyInstance()->bLockXRotation) {
				NewRotator.Pitch = MotionTracker->GetComponentRotation().Pitch;
			} else {
				NewRotator.Pitch = Item->GetComponentRotation().Pitch;
			}

			if (!Item->GetBodyInstance()->bLockZRotation) {
				NewRotator.Yaw = MotionTracker->GetComponentRotation().Yaw;
			} else {
				NewRotator.Yaw = Item->GetComponentRotation().Yaw;
			}

			if (!Item->GetBodyInstance()->bLockYRotation) {
				NewRotator.Roll = MotionTracker->GetComponentRotation().Roll;
			} else {
				NewRotator.Roll = Item->GetComponentRotation().Roll;
			}

			Item->SetWorldRotation(NewRotator, true, (FHitResult*)nullptr, ETeleportType::None);

			//Item->SetWorldRotation(FRotator(MotionTracker->GetComponentRotation().Pitch, Item->GetComponentRotation().Yaw, Item->GetComponentRotation().Roll), true, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);
			//Item->SetWorldRotation(MotionTracker->GetComponentRotation(), true, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);
		}

		//false, (FHitResult*)nullptr, ETeleportType::TeleportPhysics

		if (FVector::Dist(MotionTracker->GetComponentLocation(), Item->GetComponentLocation()) > 250.0f) {
			Drop();
		}
	}

}