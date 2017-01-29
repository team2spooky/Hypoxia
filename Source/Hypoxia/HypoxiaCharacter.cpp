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

const float MOVEMENT_SCALE = 0.3f;

AItem *HeldItemRight;
AItem *HeldItemLeft;

bool HeldRight;
bool HeldLeft;

FVector HMDPositionDelta;
FVector LastHMDPosition;

//////////////////////////////////////////////////////////////////////////
// AHypoxiaCharacter

AHypoxiaCharacter::AHypoxiaCharacter() {
	// Set size for collision capsule
	//GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);


	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	Tripod = CreateDefaultSubobject<USceneComponent>(TEXT("Tripod"));
	Tripod->SetupAttachment(RootComponent);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(Tripod);
	//FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 0.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	R_MotionTracker = CreateDefaultSubobject<USceneComponent>(TEXT("R_MotionTracker"));
	R_MotionTracker->SetupAttachment(RootComponent);
	L_MotionTracker = CreateDefaultSubobject<USceneComponent>(TEXT("L_MotionTracker"));
	L_MotionTracker->SetupAttachment(RootComponent);

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->Hand = EControllerHand::Right;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);
	L_MotionController->Hand = EControllerHand::Left;

	R_MotionController->bAbsoluteLocation = true;
	R_MotionController->bAbsoluteRotation = true;
	L_MotionController->bAbsoluteLocation = true;
	L_MotionController->bAbsoluteRotation = true;
	
}

void AHypoxiaCharacter::BeginPlay() {
	
	Super::BeginPlay();

	//UE_LOG(LogTemp, Warning, TEXT("Your message"));

	bUsingMotionControllers = true;

	FirstPersonCameraComponent->bLockToHmd = false;
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	HeldRight = false;
	HeldLeft  = false;

	HeldItemRight = NULL;
	HeldItemLeft  = NULL;

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

	//SetActorRotation(FRotator(GetCapsuleComponent()->RelativeRotation.Pitch, FirstPersonCameraComponent->RelativeRotation.Yaw, GetCapsuleComponent()->RelativeRotation.Roll));
	
	//SetActorLocation(FVector(FirstPersonCameraComponent->RelativeLocation.X, FirstPersonCameraComponent->RelativeLocation.Y, GetCapsuleComponent()->RelativeLocation.Z));
	//SetActorRotation(FirstPersonCameraComponent->RelativeRotation);
	//GetCapsuleComponent()->SetRelativeRotation(FRotator(FirstPersonCameraComponent->RelativeRotation.Pitch, FirstPersonCameraComponent->RelativeRotation.Roll, GetCapsuleComponent()->RelativeRotation.Yaw));
	//FirstPersonCameraComponent->RelativeRotation.Roll
	if (Value != 0.0f)
	{
		// add movement in that direction		
		//AddMovementInput(GetActorForwardVector(), Value);

		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AHypoxiaCharacter::MoveRight(float Value) {

	//SetActorRotation(FRotator(GetCapsuleComponent()->RelativeRotation.Pitch, FirstPersonCameraComponent->RelativeRotation.Yaw, GetCapsuleComponent()->RelativeRotation.Roll));
	
	//SetActorLocation(FVector(FirstPersonCameraComponent->RelativeLocation.X, FirstPersonCameraComponent->RelativeLocation.Y, GetCapsuleComponent()->RelativeLocation.Z));
	//SetActorRotation(FirstPersonCameraComponent->RelativeRotation);
	//SetActorLocation(FRotator(FirstPersonCameraComponent->RelativeRotation.Pitch, FirstPersonCameraComponent->RelativeRotation.Yaw, FirstPersonCameraComponent->RelativeRotation.Roll));

	if (Value != 0.0f)
	{
		// add movement in that direction

		AddMovementInput(GetActorRightVector(), Value);

		//AddMovementInput(DeviceRotation.RotateVector(GetActorRightVector()), Value);
		//AddMovementInput(FRotator(FirstPersonCameraComponent->RelativeRotation.Pitch, FirstPersonCameraComponent->RelativeRotation.Roll, GetCapsuleComponent()->RelativeRotation.Yaw).RotateVector(GetActorRightVector()), Value);
	}
}

void AHypoxiaCharacter::ItemPickupRight() {

	if (!HeldRight) {
		for (TActorIterator<AItem> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
			ActorItr->Pickup(R_MotionTracker, EControllerHand::Right);
			HeldRight = true;
		    break;
		}
	} else {
		HeldItemRight->Drop();
		HeldItemRight = NULL;
		HeldRight = false;
	}
}

void AHypoxiaCharacter::ItemPickupLeft() {

	if (!HeldLeft) {
		for (TActorIterator<AItem> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
			ActorItr->Pickup(L_MotionTracker, EControllerHand::Left);
			HeldLeft = true;
			//break;
		}
	} else {
		HeldItemLeft->Drop();
		HeldItemLeft = NULL;
		HeldLeft = false;
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
		HeldItemLeft  = Item;
	} else {
		UE_LOG(LogTemp, Error, TEXT("Invaild Hand to hold Item"));
	}
}

void AHypoxiaCharacter::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);

	FVector  DevicePosition;
	FRotator DeviceRotation;

	UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(DeviceRotation, DevicePosition);

	HMDPositionDelta = DevicePosition - LastHMDPosition;
	LastHMDPosition  = DevicePosition;

	AddMovementInput(HMDPositionDelta, MOVEMENT_SCALE);

	FirstPersonCameraComponent->SetWorldRotation(DeviceRotation);

	//RootComponent->SetWorldLocation(DevicePosition);

	//R_MotionController->SetWorldLocation(RootComponent->GetComponentLocation());

	//UE_LOG(LogTemp, Error, TEXT("Position  X: %f"), RootComponent->GetComponentLocation().X);
	
	/*UE_LOG(LogTemp, Error, TEXT("Component X: %f"), R_MotionController->GetComponentLocation().X);
	UE_LOG(LogTemp, Error, TEXT("Component Y: %f"), R_MotionController->GetComponentLocation().Y);
	UE_LOG(LogTemp, Error, TEXT("Component Z: %f"), R_MotionController->GetComponentLocation().Z);*/

	R_MotionTracker->SetWorldLocation(R_MotionController->GetComponentLocation() + RootComponent->GetComponentLocation() - DevicePosition + FVector(0.0f, 0.0f, 60.0f));
	R_MotionTracker->SetWorldRotation(R_MotionController->GetComponentRotation() + RootComponent->GetComponentRotation() - FRotator(0.0f, DeviceRotation.Yaw, 0.0f));
	L_MotionTracker->SetWorldLocation(L_MotionController->GetComponentLocation() + RootComponent->GetComponentLocation() - DevicePosition + FVector(0.0f, 0.0f, 60.0f));
	L_MotionTracker->SetWorldRotation(L_MotionController->GetComponentRotation() + RootComponent->GetComponentRotation() - FRotator(0.0f, DeviceRotation.Yaw, 0.0f));

	/*UE_LOG(LogTemp, Error, TEXT("Component X: %f"), R_MotionTracker->GetComponentLocation().X);
	UE_LOG(LogTemp, Error, TEXT("Component Y: %f"), R_MotionTracker->GetComponentLocation().Y);
	UE_LOG(LogTemp, Error, TEXT("Component Z: %f"), R_MotionTracker->GetComponentLocation().Z);*/

	//FirstPersonCameraComponent->SetWorldLocation(GetCapsuleComponent()->GetComponentLocation());
	//FirstPersonCameraComponent->SetWorldLocation(FVector(HMDPositionDelta.X + GetCapsuleComponent()->GetComponentLocation().X, HMDPositionDelta.Y + GetCapsuleComponent()->GetComponentLocation().Y, GetRootComponent()->GetComponentLocation().Z));
	
	//UE_LOG(LogTemp, Error, TEXT("Camera X: %f"), FirstPersonCameraComponent->GetComponentLocation().X);

	//UE_LOG(LogTemp, Error, TEXT("Camera X: %f"), FirstPersonCameraComponent->GetComponentLocation().X);

	//UE_LOG(LogTemp, Error, TEXT("Root Name: %s"), *RootComponent->GetName());
	//UE_LOG(LogTemp, Error, TEXT("Coom Name: %s"), *GetCapsuleComponent()->GetName());

	//FirstPersonCameraComponent->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));
	//SetActorRotation(DeviceRotation);

	//UE_LOG(LogTemp, Error, TEXT("RMotionLocation %f"), R_MotionController->GetComponentLocation().X);

	//FirstPersonCameraComponent->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));
}