// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PowerableObject.h"
#include "SlidingDoor.generated.h"

/**
 * 
 */
UCLASS()
class HYPOXIA_API ASlidingDoor : public APowerableObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASlidingDoor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	virtual void Power(float Strength) override;

	UFUNCTION(BlueprintCallable, Category = "Door")
	void Open(float Percentage = 1.f);

	UFUNCTION(BlueprintCallable, Category = "Door")
	void Close(float Percentage = 1.f);

	UFUNCTION(BlueprintCallable, Category = "Door")
	void SetLocked(bool NewState);

	UPROPERTY(EditAnywhere, Category = "Door")
	bool Locked = false;

	/* Speed at which the door opens */
	UPROPERTY(EditAnywhere, Category = "Door")
	float Speed = 0.005;

	/* Width of the door */
	UPROPERTY(EditAnywhere, Category = "Door")
	float OpenDistance = 160.f;

	/* Where along its position will the door start. 0 is closed and 1 is open. */
	UPROPERTY(EditAnywhere, Category = "Door")
	float StartingPoint = 0.f;

	float Opened = 0.f;

private:
	// Value between 0 and 1 inclusive
	float TargetPosition = 0.f;

	FVector ClosedPosition;
	
};
