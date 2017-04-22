// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item.h"
#include "Runtime/Engine/Classes/PhysicsEngine/PhysicsConstraintComponent.h"
#include "SlidingSwitch.generated.h"

/**
 * 
 */
UCLASS()
class HYPOXIA_API ASlidingSwitch : public AItem
{
	GENERATED_BODY()
	
public:
	ASlidingSwitch();
	
protected:
	UPROPERTY(VisibleAnywhere, Category = Mesh)
	class UStaticMeshComponent* Base;
	
	UPROPERTY(VisibleAnywhere)
	class UPhysicsConstraintComponent* Constraint;
};
