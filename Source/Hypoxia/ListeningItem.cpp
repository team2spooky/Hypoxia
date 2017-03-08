// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "ListeningItem.h"

void AListeningItem::Hear(float Volume) { check(0 && "You must override this function"); }

UComplexAudioComponent* AListeningItem::GenerateOnHitSound() {
	UComplexAudioComponent* HitSound = Super::GenerateOnHitSound();
	HitSound->bListenToSelf = this->bListenToSelf;
	return HitSound;
}
