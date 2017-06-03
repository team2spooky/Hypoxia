// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PowerableObject.h"
#include "GameFramework/Actor.h"
#include "PlantSocket.generated.h"

UCLASS()
class HYPOXIA_API APlantSocket : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, Category = Mesh)
	class UStaticMeshComponent* StaticMesh;
	
public:	
	// Sets default values for this actor's properties
	APlantSocket();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void Power(float Volume);
	
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* SnapSpot;

	UPROPERTY(EditAnywhere)
	float SnapDistance = 100.f;

	UPROPERTY(EditAnywhere)
	float AngleTolerance = 45.f;

	UPROPERTY(EditAnywhere)
		bool bDrainPower = false;

	APowerableObject* TargetObject;
	float ObjectPower;
};
