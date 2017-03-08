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
	void Use() override;
	
private:
	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* AudioLog;

};
