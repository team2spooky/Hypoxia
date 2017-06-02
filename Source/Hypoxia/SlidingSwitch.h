// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item.h"
#include "Runtime/Engine/Classes/PhysicsEngine/PhysicsConstraintComponent.h"
#include "SlidingSwitch.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSwitchOn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSwitchOff);

UCLASS()
class HYPOXIA_API ASlidingSwitch : public AItem
{
	GENERATED_BODY()
	
public:
	ASlidingSwitch();

	virtual void BeginPlay() override;

	virtual void Drop() override;

	UFUNCTION(BlueprintNativeEvent)
	void SwitchOn();
	virtual void SwitchOn_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void SwitchOff();
	virtual void SwitchOff_Implementation();

	UPROPERTY(BlueprintAssignable, Category = Switch)
	FSwitchOn OnSwitchOn;

	UPROPERTY(BlueprintAssignable, Category = Switch)
	FSwitchOff OnSwitchOff;
	
protected:
	UPROPERTY(VisibleAnywhere, Category = Mesh)
	class UStaticMeshComponent* Base;
	
	UPROPERTY(VisibleAnywhere)
	class UPhysicsConstraintComponent* Constraint;

private:
	float TravelDistance = 25.f;

	UPROPERTY(EditAnywhere)
	bool bOn = false;
};
