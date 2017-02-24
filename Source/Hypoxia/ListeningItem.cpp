// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "ListeningItem.h"

void AListeningItem::Hear(float volume) {}

UComplexAudioComponent* AListeningItem::GenerateOnHitSound() {
	UComplexAudioComponent* HitSound = Super::GenerateOnHitSound();
	HitSound->bListenToSelf = this->bListenToSelf;
	return HitSound;
}
