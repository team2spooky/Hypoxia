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

	bool Opened = false;

	bool Moving = false;

public:

	virtual void BeginPlay() override;
	
protected:

	ADoor();

	virtual void Tick(float) override;

	virtual void Use() override;

	void Open();

	void Close();

	virtual void UpdateRotation(FRotator) override;

};
