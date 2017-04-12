// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "SlidingDoor.h"


ASlidingDoor::ASlidingDoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASlidingDoor::BeginPlay()
{
	Super::BeginPlay();
	ClosedPosition = this->GetActorLocation();
}

// Called every frame
void ASlidingDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (FMath::Abs(Opened - TargetPosition) > 0.02) {
		Opened += 0.005f * FMath::Sign(TargetPosition - Opened);
	}
	this->SetActorLocation(ClosedPosition + this->GetActorForwardVector() * 160.f * Opened);
}

void ASlidingDoor::Power(float Strength) {
	if (Strength != 0) {
		Open(Strength);
	} else {
		Close(1.0f);
	}
}

void ASlidingDoor::Open(float Percentage)
{
	TargetPosition = Percentage;
}

void ASlidingDoor::Close(float Percentage)
{
	TargetPosition = 1 - Percentage;
}

