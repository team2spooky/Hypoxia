// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "InteractiveItem.h"

bool AInteractiveItem::Pickup(USceneComponent* Controller, EControllerHand Hand) {
	if (FVector::Dist(Controller->GetComponentLocation(), GrabSpot->GetComponentLocation()) < 50.0f) {
		this->Use();
	}
	return false;
}


