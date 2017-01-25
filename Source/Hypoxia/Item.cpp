// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "Item.h"
#include "MotionControllerComponent.h"
#include "HypoxiaCharacter.h"
#include "EngineUtils.h"

AHypoxiaCharacter *HypoxiaCharacter;

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	Item_Base = CreateDefaultSubobject<USceneComponent>(TEXT("Item_Base"));

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	MotionController->Hand = EControllerHand::Right;
	MotionController->SetupAttachment(Item_Base);

	Item = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Item"));
	Item->SetupAttachment(Item_Base);
	Item->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	//MotionController->Hand = EControllerHand::Right;

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AHypoxiaCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{

		HypoxiaCharacter = *ActorItr;
		//break;
	}

}

void AItem::Pickup() {
	//MotionController->Hand = Hand;
	//FVector HandLocation = MotionController->GetComponentLocation();

	if (FVector::Dist(MotionController->GetComponentLocation(), Item_Base->GetComponentLocation()) < 100000.0f) {

		UE_LOG(LogTemp, Warning, TEXT("Thing"));

		if (Item_Base->AttachToComponent(HypoxiaCharacter->GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale)) {
			HypoxiaCharacter->SetHeldItem(this);
			Item->AttachToComponent(MotionController, FAttachmentTransformRules::SnapToTargetIncludingScale);
			UE_LOG(LogTemp, Warning, TEXT("It worked :)"));
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("It no worked :("));
		}
	}
}

void AItem::Drop() {
	Item_Base->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	Item->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}

// Called every frame
void AItem::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

