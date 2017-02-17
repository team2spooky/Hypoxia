// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item.h"
#include "Door.h"

#include "Keycard.generated.h"

/**
 * 
 */
UCLASS()
class HYPOXIA_API AKeycard : public AItem
{
	GENERATED_BODY()
	
	
public:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

		//void UnlockLock();
	
protected:

	ADoor *Door;

	virtual void Tick(float) override;
};
