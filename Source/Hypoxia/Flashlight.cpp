// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "Flashlight.h"
#include "MotionControllerComponent.h"

AFlashlight::AFlashlight()
{

	//Light_MotionController = GetDefaultSubobjectByName(TEXT("MotionController"));

	Light = CreateDefaultSubobject<USpotLightComponent>(TEXT("Light"));
	Light->SetupAttachment(Item);
}

void AFlashlight::Use() {
	
	float intensity = Light->Intensity;
	if (intensity == 0.0f) {
		Light->SetIntensity(1000000.0f);
	} else {
		Light->SetIntensity(0.0f);
	}

	//Rotation_Time = 240;

	//Drop();
	//Item->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f), false, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);
}

void AFlashlight::Tick(float DeltaTime) {
	
	Super::Tick(DeltaTime);

	/*if (Rotation_Time >= 0) {
		Rotation_Time--;
		Item->SetRelativeRotation(FRotator(Item->GetComponentRotation().Pitch + 1.0f, 0.0f, 0.0f), false, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);
	}*/
}
