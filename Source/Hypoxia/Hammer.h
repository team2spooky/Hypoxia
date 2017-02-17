// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item.h"
#include "PhysicsEngine/DestructibleActor.h"
#include "Hammer.generated.h"

/**
 * 
 */
UCLASS()
class HYPOXIA_API AHammer : public AItem
{
	GENERATED_BODY()
	

public:

	virtual void BeginPlay() override;

protected:

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	
	
};
