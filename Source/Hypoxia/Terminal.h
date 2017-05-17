// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractiveItem.h"
#include "Terminal.generated.h"

/**
 * 
 */
UCLASS()
class HYPOXIA_API ATerminal : public AInteractiveItem
{
	GENERATED_BODY()
	
public:
	ATerminal();

	void Use() override;

protected:
	UPROPERTY(BlueprintReadOnly)
	UAudioComponent* AudioPlayer;
	
private:
	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* AudioLog;

};
