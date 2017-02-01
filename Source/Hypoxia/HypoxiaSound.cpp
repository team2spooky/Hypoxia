// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "HypoxiaSound.h"

void UHypoxiaSound::PlaySoundAtLocationRefract(const UObject* WorldContextObject, USoundBase* Sound, FVector Location, float VolumeMultiplier, float PitchMultiplier, float StartTime, USoundAttenuation* Attenuation) {
	UGameplayStatics::PlaySoundAtLocation(WorldContextObject, Sound, Location, VolumeMultiplier, PitchMultiplier, StartTime, Attenuation);
}

void UHypoxiaSound::PlayerSoundAt(const UObject* WorldContextObject, FVector Location, float Volume) {

}


