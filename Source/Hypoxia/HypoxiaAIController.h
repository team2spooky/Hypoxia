// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "HypoxiaAIController.generated.h"

/**
 * 
 */
UCLASS()
class HYPOXIA_API AHypoxiaAIController : public AAIController
{
	GENERATED_BODY()

public:
	void TrackPlayer(float Volume);
	
	
};
