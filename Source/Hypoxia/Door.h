// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item.h"
#include "Door.generated.h"

/**
 * 
 */
UCLASS()
class HYPOXIA_API ADoor : public AItem
{
	GENERATED_BODY()
	
public:
	
	virtual void BeginPlay() override;
	
protected:

	//virtual void Tick(float) override;

	virtual void Use() override;

};
