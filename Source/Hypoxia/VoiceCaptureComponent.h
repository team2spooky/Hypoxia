// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Runtime/Online/Voice/Public/Voice.h"
#include "VoiceCaptureComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HYPOXIA_API UVoiceCaptureComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVoiceCaptureComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	void TriggerObjects(float volume);

private:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* InfluenceSphere;

	TSharedPtr<IVoiceCapture> VoiceCapture;
	TArray<uint8> VoiceCaptureBuffer;
	// voice playback variables
	/*
	UAudioComponent* VoiceCaptureAudioComponent;
	USoundWaveProcedural* VoiceCaptureSoundWaveProcedural;
	bool PlayVoiceCaptureFlag;
	*/
};
