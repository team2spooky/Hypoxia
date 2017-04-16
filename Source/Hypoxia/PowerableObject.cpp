// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "PowerableObject.h"


// Sets default values
APowerableObject::APowerableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = StaticMesh;
}

// Called when the game starts or when spawned
void APowerableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APowerableObject::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void APowerableObject::Power(float Strength) {}

