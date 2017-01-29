// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "DoorHandle.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"

int RotationTime = 0;

ADoorHandle::ADoorHandle()
{

	//Light_MotionController = GetDefaultSubobjectByName(TEXT("MotionController"));

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(Item);
}

void ADoorHandle::Use() {
	RotationTime = 240;
}

bool ADoorHandle::Pickup(USceneComponent* Controller, EControllerHand Hand) {
	
	FVector PrevLocation = Item->GetComponentLocation();

	bool Pickedup = Super::Pickup(Controller, Hand);

	Item->SetWorldLocation(PrevLocation);

	return Pickedup;
}

void ADoorHandle::Tick(float DeltaTime) {
	//Super::Tick(DeltaTime);

	if (RotationTime > 0) {
		RotationTime--;

	}
}