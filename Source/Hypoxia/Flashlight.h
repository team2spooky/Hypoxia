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

	UPROPERTY(VisibleAnywhere, Category = Text)
	class UTextRenderComponent* Text;

	UPROPERTY(EditAnywhere, Category = Sound)
	class USoundBase* ClickOnSound;

	UPROPERTY(EditAnywhere, Category = Sound)
	class USoundBase* ClickOffSound;
	

public:

	AFlashlight();

	virtual void Use();

protected:

	//virtual void Tick(float) override;

};
