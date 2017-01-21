// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "VR_Character.h"


// Sets default values
AVR_Character::AVR_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVR_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVR_Character::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AVR_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

