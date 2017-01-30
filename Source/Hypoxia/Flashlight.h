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

public:

	AFlashlight();

	virtual void Use();

protected:

	//virtual void Tick(float) override;

};
