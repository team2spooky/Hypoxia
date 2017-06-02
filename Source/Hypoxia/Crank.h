// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item.h"
#include "Runtime/Engine/Classes/PhysicsEngine/PhysicsConstraintComponent.h"
#include "Crank.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCrankTick);

UCLASS()
class HYPOXIA_API ACrank : public AItem
{
	GENERATED_BODY()
	
public:
	ACrank();

	virtual void BeginPlay() override;

	virtual void Drop() override;

	UFUNCTION(BlueprintNativeEvent)
	void CrankOn();
	virtual void CrankOn_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void CrankOff();
	virtual void CrankOff_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void CrankTick();
	virtual void CrankTick_Implementation();

	UFUNCTION(BlueprintCallable, Category = Crank)
	float GetPercent();

	UPROPERTY(BlueprintAssignable, Category = Crank)
	FCrankTick OnCrankTick;

protected:
	UPROPERTY(VisibleAnywhere, Category = Mesh)
		class UStaticMeshComponent* Base;

	UPROPERTY(VisibleAnywhere)
		class UPhysicsConstraintComponent* Constraint;
	
	virtual void Tick(float DeltaTime) override;

private:
	float NormalizeAngle(float);

	float Angle = 0.f;
	float AngleTraveled = 0.f;

	float PrevUpdate = 0.f;
	float NextUpdate = 0.f;

	UPROPERTY(EditAnywhere)
	float UpdateAngle = 15.0f;

	UPROPERTY(EditAnywhere)
	bool bOn = false;

	UPROPERTY(EditAnywhere)
	float MinAngle = -360.f;

	UPROPERTY(EditAnywhere)
	float MaxAngle = 360.f;

	UPROPERTY(EditAnywhere)
	float AngleTolerance = 5.0f;
	
};
