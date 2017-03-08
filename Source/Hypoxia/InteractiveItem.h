// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item.h"
#include "InteractiveItem.generated.h"

/**
 * 
 */
UCLASS(abstract)
class HYPOXIA_API AInteractiveItem : public AItem
{
	GENERATED_BODY()
	
public:
	virtual bool Pickup(USceneComponent*, EControllerHand) override;
	
	
};
