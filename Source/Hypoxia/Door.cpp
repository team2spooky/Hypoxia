// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "Door.h"

void ADoor::BeginPlay() {

	Super::BeginPlay();

	Item->SetEnableGravity(false);
	Item->GetBodyInstance()->bLockTranslation = true;
	Item->GetBodyInstance()->bLockRotation = false;
	Item->GetBodyInstance()->bLockXRotation = false;
	Item->GetBodyInstance()->bLockYRotation = true;
	Item->GetBodyInstance()->bLockZRotation = true;

}

void ADoor::Use() {

	//Rotation_Time = 240;

	//Drop();
	Item->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f), false, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);
}

//void ADoor::Tick(float DeltaTime) {
//	Super::Tick(DeltaTime);
//}