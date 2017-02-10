// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MotionControllerComponent.h"
#include "Item.generated.h"

UCLASS()
class HYPOXIA_API AItem : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = Mesh)
	class USceneComponent* Item_Base;

	UPROPERTY(VisibleAnywhere, Category = Mesh)
	class UStaticMeshComponent* Item;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* MotionController;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* MotionTracker;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* GrabSpot;

	bool Held;

	FVector OldLocation;

	FVector NewLocation;
	
public:
	// Sets default values for this actor's properties

	void Drop();

	virtual void Use();

	virtual bool Pickup(USceneComponent*, EControllerHand);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	bool bHasGravity;

	//Portected constuctor as AItem should never be instantiated
	AItem();

	virtual void Tick(float) override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	
	virtual void UpdatePosition(FVector);

	virtual void UpdateRotation(FRotator);

	void SelfDrop();

};
