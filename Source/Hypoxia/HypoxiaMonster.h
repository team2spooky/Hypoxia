// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ComplexAudioComponent.h"
#include "GameFramework/Character.h"
#include "HypoxiaMonster.generated.h"

UCLASS()
class HYPOXIA_API AHypoxiaMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHypoxiaMonster();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PlayDetectSound();

private:
	UComplexAudioComponent* MonsterSound;

	float IdleSoundTimer;

	UPROPERTY(EditAnywhere)
	USoundAttenuation* SoundAttenuation;

	UPROPERTY(EditAnywhere)
	USoundBase* IdleSound;

	UPROPERTY(EditAnywhere)
	USoundBase* DetectSound;

	UPROPERTY(EditAnywhere, Category="Sound|Idle")
	float IdleSoundDelay;

	UPROPERTY(EditAnywhere, Category = "Sound|Idle")
	float IdleSoundDelayRandomness;
};
