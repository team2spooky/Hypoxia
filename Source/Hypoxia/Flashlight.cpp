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
		Light->SetIntensity(10000000.0f);
	} else {
		Light->SetIntensity(0.0f);
	}
}
