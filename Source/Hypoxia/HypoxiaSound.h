// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "HypoxiaSound.generated.h"

/**
 * 
 */
UCLASS()
class HYPOXIA_API UHypoxiaSound : public UObject
{
	GENERATED_BODY()
	
public:
	/* Refracts sound to player */
	static void PlaySoundAtLocationRefract(const UObject* WorldContextObject, USoundBase* Sound, FVector Location, float VolumeMultiplier, float PitchMultiplier, float StartTime, USoundAttenuation* Attenuation);
	
	/* Processes any sounds made by player*/
	static void PlayerSoundAt(const UObject* WorldContextObject, FVector Location, float Volume);


};
