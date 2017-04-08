// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SlidingDoor.generated.h"

UCLASS()
class HYPOXIA_API ASlidingDoor : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, Category = Mesh)
	class UStaticMeshComponent* StaticMesh;
	
public:	
	// Sets default values for this actor's properties
	ASlidingDoor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void Open(float Percentage = 1.f);

	void Close(float Percentage = 1.f);

	UPROPERTY()
	bool Locked = false;

	float Opened = 0.f;

private:
	// Value between 0 and 1 inclusive
	float TargetPosition = 0.f;

	FVector ClosedPosition;
};
