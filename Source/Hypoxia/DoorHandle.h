// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item.h"
#include "DoorHandle.generated.h"

/**
 * 
 */
UCLASS()
class HYPOXIA_API ADoorHandle : public AItem
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = Mesh)
	class UStaticMeshComponent* Door;
	
public:

	ADoorHandle();

	virtual void Use();

	bool Pickup(USceneComponent*, EControllerHand) override;

protected:

	virtual void Tick(float) override;
	
};
