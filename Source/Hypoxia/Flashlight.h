// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item.h"
#include "Door.h"
#include "Flashlight.generated.h"

/**
 * 
 */


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMyBindableEvent);

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

	virtual void BeginPlay() override;

	virtual bool Pickup(USceneComponent*, EControllerHand);

	UFUNCTION(BlueprintCallable, Category = Flashlight)
	void KillLight();

	UPROPERTY(BlueprintAssignable)
	FMyBindableEvent OnPickUp;

protected:

	ADoor *Door;

	bool Alive;

	UMaterialInstanceDynamic* DynamicMaterial;
};
