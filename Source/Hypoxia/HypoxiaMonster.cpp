// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "HypoxiaMonster.h"

// Sets default values
AHypoxiaMonster::AHypoxiaMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MonsterSound = CreateDefaultSubobject<UComplexAudioComponent>(FName("Monster Sound"));
	MonsterSound->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AHypoxiaMonster::BeginPlay()
{
	Super::BeginPlay();
	MonsterSound->SetAttenuationSettings(SoundAttenuation);
	IdleSoundTimer = IdleSoundDelay + FMath::RandRange(-0.5f, 1.0f) * IdleSoundDelayRandomness;
}

// Called every frame
void AHypoxiaMonster::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	IdleSoundTimer -= DeltaTime;
	if (IdleSoundTimer <= 0) {
		MonsterSound->SetSound(IdleSound);
		MonsterSound->Play();
		IdleSoundTimer = IdleSoundDelay + FMath::RandRange(-0.5f, 1.0f) * IdleSoundDelayRandomness;
	}
}

// Called to bind functionality to input
void AHypoxiaMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AHypoxiaMonster::PlayDetectSound() {
	MonsterSound->SetSound(DetectSound);
	MonsterSound->Play();
	IdleSoundTimer = 10000.f;
}

