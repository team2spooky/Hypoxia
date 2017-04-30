// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item.h"
#include "Runtime/Engine/Classes/Components/DestructibleComponent.h"
#include "DestructibleJunk.generated.h"

/**
 * 
 */
UCLASS()
class HYPOXIA_API ADestructibleJunk : public AItem
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere)
	UDestructibleComponent* DestructibleMesh;
	
public:
	ADestructibleJunk();

	//virtual void BeginPlay() override;

	/*virtual void Tick(float) override;

	virtual void Drop() override;*/

	virtual bool Pickup(USceneComponent*, EControllerHand) override;

	virtual void DoHit(FVector ImpactPoint, FVector NormalImpulse) override;

	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) override;

private:
	bool Broken = false;
};
