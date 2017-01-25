// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item.h"
#include "Flashlight.generated.h"

/**
 * 
 */
UCLASS()
class HYPOXIA_API AFlashlight : public AItem
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, Category = Light)
	class USpotLightComponent* Light;
	
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* Light_MotionController;

public:

	AFlashlight();

};
