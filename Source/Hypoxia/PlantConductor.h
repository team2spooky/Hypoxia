// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ListeningItem.h"
#include "PlantSocket.h"
#include "PlantConductor.generated.h"

/**
 * 
 */
UCLASS()
class HYPOXIA_API APlantConductor : public AListeningItem
{
	GENERATED_BODY()
	
public:
	APlantConductor();

	virtual void BeginPlay() override;
	virtual void Tick(float) override;

	virtual void Drop() override;
	virtual bool Pickup(USceneComponent*, EControllerHand) override;

	virtual void Hear(float volume) override;

	float GlowIntensity = 0.0f;

private:
	UMaterialInstanceDynamic* DynamicMaterial;
	UParticleSystemComponent* Particles;
	UPointLightComponent* GlowLight;

	APlantSocket* Socket;
};
