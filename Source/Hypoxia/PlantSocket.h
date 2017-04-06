// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlantConductor.h"
#include "GameFramework/Actor.h"
#include "PlantSocket.generated.h"

UCLASS()
class HYPOXIA_API APlantSocket : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlantSocket();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	APlantConductor* Conductor;
	
};
