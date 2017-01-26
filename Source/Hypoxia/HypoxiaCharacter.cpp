// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Hypoxia.h"
#include "HypoxiaCharacter.h"
#include "HypoxiaProjectile.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"
#include "Item.h"
#include "EngineUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

AItem *HeldItemRight = NULL;
AItem *HeldItemLeft  = NULL;

//////////////////////////////////////////////////////////////////////////
// AHypoxiaCharacter

AHypoxiaCharacter::AHypoxiaCharacter() {
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 0.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = false;

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->Hand = EControllerHand::Right;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);
	L_MotionController->Hand = EControllerHand::Left;
	
}

void AHypoxiaCharacter::BeginPlay() {
	
	Super::BeginPlay();

	//UE_LOG(LogTemp, Warning, TEXT("Your message"));

	bUsingMotionControllers = true;

}

//////////////////////////////////////////////////////////////////////////
// Input

void AHypoxiaCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {

	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AHypoxiaCharacter::TouchStarted);
	/*if (EnableTouchscreenMovement(PlayerInputComponent) == false)
	{
		PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AHypoxiaCharacter::OnFire);
	}*/

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AHypoxiaCharacter::OnResetVR);

	PlayerInputComponent->BindAxis("MoveForward", this, &AHypoxiaCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHypoxiaCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	//PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	//PlayerInputComponent->BindAxis("TurnRate", this, &AHypoxiaCharacter::TurnAtRate);
	//PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	//PlayerInputComponent->BindAxis("LookUpRate", this, &AHypoxiaCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("Pickup_Right", IE_Pressed, this, &AHypoxiaCharacter::ItemPickupRight);
	PlayerInputComponent->BindAction("Pickup_Left" , IE_Pressed, this, &AHypoxiaCharacter::ItemPickupLeft );

	PlayerInputComponent->BindAction("Use_Right"   , IE_Pressed, this, &AHypoxiaCharacter::ItemUseRight);
	PlayerInputComponent->BindAction("Use_Left"    , IE_Pressed, this, &AHypoxiaCharacter::ItemUseLeft);
	
	//PlayerInputComponent->BindAction("Use", IE_Pressed, this, &AHypoxiaCharacter::FlashlightOnOff);
}

void AHypoxiaCharacter::OnResetVR() {

	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AHypoxiaCharacter::MoveForward(float Value) {

	SetActorRotation(FRotator(GetCapsuleComponent()->RelativeRotation.Pitch, FirstPersonCameraComponent->RelativeRotation.Yaw, GetCapsuleComponent()->RelativeRotation.Roll));
	//SetActorLocation(FVector(FirstPersonCameraComponent->RelativeLocation.X, FirstPersonCameraComponent->RelativeLocation.Y, GetCapsuleComponent()->RelativeLocation.Z));
	//SetActorRotation(FirstPersonCameraComponent->RelativeRotation);
	//GetCapsuleComponent()->SetRelativeRotation(FRotator(FirstPersonCameraComponent->RelativeRotation.Pitch, FirstPersonCameraComponent->RelativeRotation.Roll, GetCapsuleComponent()->RelativeRotation.Yaw));
	//FirstPersonCameraComponent->RelativeRotation.Roll
	if (Value != 0.0f)
	{
		// add movement in that direction		
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AHypoxiaCharacter::MoveRight(float Value) {

	SetActorRotation(FRotator(GetCapsuleComponent()->RelativeRotation.Pitch, FirstPersonCameraComponent->RelativeRotation.Yaw, GetCapsuleComponent()->RelativeRotation.Roll));
	//SetActorLocation(FVector(FirstPersonCameraComponent->RelativeLocation.X, FirstPersonCameraComponent->RelativeLocation.Y, GetCapsuleComponent()->RelativeLocation.Z));
	//SetActorRotation(FirstPersonCameraComponent->RelativeRotation);
	//SetActorLocation(FRotator(FirstPersonCameraComponent->RelativeRotation.Pitch, FirstPersonCameraComponent->RelativeRotation.Yaw, FirstPersonCameraComponent->RelativeRotation.Roll));

	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
		//AddMovementInput(FRotator(FirstPersonCameraComponent->RelativeRotation.Pitch, FirstPersonCameraComponent->RelativeRotation.Roll, GetCapsuleComponent()->RelativeRotation.Yaw).RotateVector(GetActorRightVector()), Value);
	}
}

void AHypoxiaCharacter::ItemPickupRight() {

	if (HeldItemRight == NULL) {
		for (TActorIterator<AItem> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			ActorItr->Pickup(R_MotionController);
		    break;
		}
		
	} else {
		HeldItemRight->Drop();
		HeldItemRight = NULL;
	}
}

void AHypoxiaCharacter::ItemPickupLeft() {

	if (HeldItemLeft == NULL) {
		for (TActorIterator<AItem> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			ActorItr->Pickup(L_MotionController);
			break;
		}

	}
	else {
		HeldItemLeft->Drop();
		HeldItemLeft = NULL;
	}

}

void AHypoxiaCharacter::ItemUseRight() {
	if (HeldItemRight != NULL) {
		HeldItemRight->Use();
	}
}

void AHypoxiaCharacter::ItemUseLeft() {
	if (HeldItemLeft != NULL) {
		HeldItemLeft->Use();
	}
}

void AHypoxiaCharacter::SetHeldItem(AItem* Item, EControllerHand Hand) {
	if (Hand == EControllerHand::Right) {
		HeldItemRight = Item;
	} else if(Hand == EControllerHand::Left) {
		HeldItemLeft = Item;
	} else {
		UE_LOG(LogTemp, Error, TEXT("Invaild Hand to hold Item"));
	}
}
