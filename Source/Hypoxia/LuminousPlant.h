// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ListeningItem.h"
#include "LuminousPlant.generated.h"

/**
 * 
 */
UCLASS()
class HYPOXIA_API ALuminousPlant : public AListeningItem
{
	GENERATED_BODY()
	
public:
	ALuminousPlant();

	virtual void BeginPlay() override;
	virtual void Tick(float) override;

	virtual void Hear(float volume) override;
	
private:
	UMaterialInstanceDynamic* DynamicMaterial;
	UParticleSystemComponent* Particles;
	
};
