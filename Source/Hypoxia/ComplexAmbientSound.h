// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ComplexAudioComponent.h"
#include "ComplexAmbientSound.generated.h"

UCLASS()
class HYPOXIA_API AComplexAmbientSound : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, NoClear)
	UComplexAudioComponent* ComplexAudioComponent;

	UPROPERTY(EditAnywhere, Category = "Sound|Ambient")
	float Delay;
	UPROPERTY(EditAnywhere, Category = "Sound|Ambient")
	float DelayRandomness;
	UPROPERTY(EditAnywhere, Category = "Sound|Ambient|Spawn")
	FVector BoxExtent;

	FVector Origin;
	FBox SpawnBox;
	float DelayTimer;
	
public:	
	// Sets default values for this actor's properties
	AComplexAmbientSound();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
};
