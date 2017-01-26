// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "Item.h"
#include "MotionControllerComponent.h"
#include "HypoxiaCharacter.h"
#include "EngineUtils.h"

AHypoxiaCharacter *HypoxiaCharacter;

bool Held = false;

// Sets default values
AItem::AItem()
{
	Item_Base = CreateDefaultSubobject<USceneComponent>(TEXT("Item_Base"));

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	MotionController->Hand = EControllerHand::Right;
	MotionController->SetupAttachment(Item_Base);

	Item = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Item"));
	Item->SetupAttachment(Item_Base);

	MotionController->Hand = EControllerHand::Left;

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AHypoxiaCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		//This may need adjusting to ensure it gets the right one
		HypoxiaCharacter = *ActorItr;
	}

}

void AItem::Pickup(UMotionControllerComponent* Controller) {
	//UE_LOG(LogTemp, Warning, TEXT("Dist %f"), FVector::Dist(Controller->GetComponentLocation(), Item_Base->GetComponentLocation()));

	if (!Held) {

		//If the item is within the given distance of the given controller, pick it up
		if (FVector::Dist(Controller->GetComponentLocation(), Item_Base->GetComponentLocation()) < 350.0f) {

			//Attach the components, don't move the item if something fails with that
			if (Item_Base->AttachToComponent(HypoxiaCharacter->GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale)) {
				HypoxiaCharacter->SetHeldItem(this, Controller->Hand);
				Item->AttachToComponent(MotionController, FAttachmentTransformRules::SnapToTargetIncludingScale);
				MotionController->Hand = Controller->Hand;
				Held = true;
				//UE_LOG(LogTemp, Warning, TEXT("It worked :)"));
			}// else {
				//UE_LOG(LogTemp, Warning, TEXT("It no worked :("));
			//}
		}
	}
}

void AItem::Drop() {
	if (Held) {
		Item_Base->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		Item->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		Held = false;
	}
}

void AItem::Use() {}

