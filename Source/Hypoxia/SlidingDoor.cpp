// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "SlidingDoor.h"


// Sets default values
ASlidingDoor::ASlidingDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = StaticMesh;
}

// Called when the game starts or when spawned
void ASlidingDoor::BeginPlay()
{
	Super::BeginPlay();
	ClosedPosition = this->GetActorLocation();
}

// Called every frame
void ASlidingDoor::Tick( float DeltaTime )
{
	Super::Tick(DeltaTime);
	Opened += 0.02f * FMath::Sign(TargetPosition);
	TargetPosition = 0;
	this->SetActorLocation(ClosedPosition + this->GetActorForwardVector() * 200.f * Opened);
}

void ASlidingDoor::Open(float Percentage)
{
	TargetPosition = Percentage - Opened;
}

void ASlidingDoor::Close(float Percentage)
{
	TargetPosition = Opened - Percentage;
}
