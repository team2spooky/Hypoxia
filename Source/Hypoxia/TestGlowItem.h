// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ListeningItem.h"
#include "TestGlowItem.generated.h"

/**
 * 
 */
UCLASS()
class HYPOXIA_API ATestGlowItem : public AListeningItem
{
	GENERATED_BODY()
	
public:
	ATestGlowItem();

	virtual void Hear(float volume) override;

	virtual void Tick(float deltaTime) override;
	
private:
	UPointLightComponent* Light;
	float GlowIntensity;

};
