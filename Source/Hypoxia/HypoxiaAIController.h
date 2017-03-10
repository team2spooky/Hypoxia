// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "HypoxiaCharacter.h"
#include "HypoxiaAIController.generated.h"

/**
 * 
 */
UCLASS(abstract)
class HYPOXIA_API AHypoxiaAIController : public AAIController
{
	GENERATED_BODY()

	//UPROPERTY(VisibleDefaultsOnly, Category = AI)
	//class UBlackboardComponent* MotionTracker;
		//public:
			//void TrackPlayer(float Volume);

	AHypoxiaCharacter *HypoxiaCharacter;

	AHypoxiaAIController();

	virtual void BeginPlay() override;

	virtual void Tick(float) override;

public:
	void HearSound(FVector, float);

};
