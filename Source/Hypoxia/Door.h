// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item.h"
#include "Engine/StaticMeshActor.h"
#include "Door.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EDoorDirection : uint8 {
	DD_North 	UMETA(DisplayName = "North"), //-Y
	DD_South 	UMETA(DisplayName = "South"), //+Y
	DD_East		UMETA(DisplayName = "East"),  //+X
	DD_West		UMETA(DisplayName = "West")   //-X
};


UCLASS()
class HYPOXIA_API ADoor : public AItem
{
	GENERATED_BODY()

	//UPROPERTY(VisibleAnywhere, Category = Mesh)
	//class UStaticMeshComponent* Door;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
	EDoorDirection DoorHandleDirection;
	
protected:

	AStaticMeshActor *Door;

	float DoorRotation = 0.0f;
	float MoveTime = 0.0f;

	bool Opened = true;
	bool Moving = true;

	//bool Moving = false;

	FVector InitialPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bool)
	bool Locked;

public:

	virtual void BeginPlay() override;
	
	void Unlock();
	
protected:

	ADoor();

	virtual void Tick(float) override;

	virtual void Use() override;

	void Open();

	void Close();

	virtual void UpdatePosition(FVector) override;

};
