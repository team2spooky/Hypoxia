// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "PoweredLight.h"

APoweredLight::APoweredLight() {
	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	Light->SetupAttachment(RootComponent);
}

void APoweredLight::Power(float Strength) {
	Light->SetIntensity(Strength * 1000);
}
