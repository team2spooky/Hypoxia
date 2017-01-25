// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "Flashlight.h"
#include "MotionControllerComponent.h"


AFlashlight::AFlashlight()
{

	//Light_MotionController = GetDefaultSubobjectByName(TEXT("MotionController"));

	Light = CreateDefaultSubobject<USpotLightComponent>(TEXT("Light"));
	Light->SetupAttachment(MotionController);
}

