// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item.h"
#include "ListeningItem.generated.h"

/**
 * 
 */
UCLASS()
class HYPOXIA_API AListeningItem : public AItem
{
	GENERATED_BODY()
	
public:
	virtual void Hear(float volume);
	
	
};
