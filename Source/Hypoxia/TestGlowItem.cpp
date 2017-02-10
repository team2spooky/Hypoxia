// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "TestGlowItem.h"

ATestGlowItem::ATestGlowItem() {
	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Glow"));
	Light->SetIntensity(0.0f);
	Light->SetupAttachment(Item);
}

void ATestGlowItem::Tick(float deltaTime) {
	GlowIntensity -= 10.0f;
	GlowIntensity = FMath::Max(0.f, GlowIntensity);
	Light->SetIntensity(GlowIntensity);
}

void ATestGlowItem::Hear(float volume) {
	GlowIntensity = FMath::Max(GlowIntensity, volume * 20.f);
}


