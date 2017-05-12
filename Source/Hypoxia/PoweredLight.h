// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PowerableObject.h"
#include "PoweredLight.generated.h"

/**
 * 
 */
UCLASS()
class HYPOXIA_API APoweredLight : public APowerableObject
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere)
	UPointLightComponent* Light;
	
public:
	APoweredLight();

	virtual void Power(float Strength) override;

};
