// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "ComplexAmbientSound.h"


// Sets default values
AComplexAmbientSound::AComplexAmbientSound()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ComplexAudioComponent = CreateDefaultSubobject<UComplexAudioComponent>(TEXT("ComplexAudioComponent"));
	ComplexAudioComponent->bAmbientSound = true;
	RootComponent = ComplexAudioComponent;

}

// Called when the game starts or when spawned
void AComplexAmbientSound::BeginPlay()
{
	Super::BeginPlay();
	
	Origin = GetActorLocation();
	FVector HalfBox = BoxExtent / 2;
	SpawnBox = FBox(Origin - HalfBox, Origin + HalfBox);

	DelayTimer = Delay + FMath::RandRange(-0.5 * DelayRandomness, DelayRandomness);

	ComplexAudioComponent->Play();
}

// Called every frame
void AComplexAmbientSound::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if (DelayTimer < 0) {
		SetActorLocation(FMath::RandPointInBox(SpawnBox));
		ComplexAudioComponent->Play();
		DelayTimer = Delay + FMath::RandRange(-0.5 * DelayRandomness, DelayRandomness);
	}
	DelayTimer -= DeltaTime;
}

