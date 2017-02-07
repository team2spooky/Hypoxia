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
	/* Initializes voice capture */
	static void InitializeVoiceCapture();

	static void CaptureVoice(UObject* WorldContextObject);

	/* Refracts sound to player */
	static void PlaySoundAtLocationRefract(const UObject* WorldContextObject, USoundBase* Sound, FVector Location, float VolumeMultiplier, float PitchMultiplier, float StartTime, USoundAttenuation* Attenuation);
	
	/* Processes any sounds made by player*/
	static void PathSoundTo(UObject* WorldContextObject, FVector StartLoc, FVector EndLoc, FVector& Loc_Out);


};
