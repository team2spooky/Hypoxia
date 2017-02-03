// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameModeBase.h"
#include "HypoxiaGameMode.generated.h"

UCLASS(minimalapi)
class AHypoxiaGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	float VoiceCaptureFinalVolume;

	AHypoxiaGameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float) override;
};

