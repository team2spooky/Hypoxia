// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class HYPOXIA_API AItem : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, Category = Mesh)
	class USceneComponent* Item_Base;

	UPROPERTY(VisibleAnywhere, Category = Mesh)
	class USkeletalMeshComponent* Item;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* MotionController;
	
public:	
	// Sets default values for this actor's properties
	AItem();

	void Drop();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

};
