// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "Door.h"
#include "EngineUtils.h"

ADoor::ADoor() {

	//Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	//Door->SetupAttachment(Item_Base);

}

void ADoor::BeginPlay() {

	Super::BeginPlay();

	for (TActorIterator<AStaticMeshActor> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		
		AStaticMeshActor *Mesh = *ActorItr;

		//UE_LOG(LogTemp, Warning, TEXT("Parent: %s"), ActorItr->Tags[0]);

		//FName Tag = FName()

		if (ActorItr->ActorHasTag(Tags[0])) {
			
			Door = *ActorItr;
			
			UE_LOG(LogTemp, Warning, TEXT("Child: %s"), *ActorItr->GetName());
			UE_LOG(LogTemp, Warning, TEXT("Parent: %s"),  *GetName());
			break;
		}
	}

	Moving = false;
	Opened = false;

	DoorRotation = 0.0f;

	//Item->SetEnableGravity(false);
	Item->GetBodyInstance()->bLockTranslation = true;
	Item->GetBodyInstance()->bLockRotation = false;
	Item->GetBodyInstance()->bLockXRotation = true;
	Item->GetBodyInstance()->bLockYRotation = true;
	Item->GetBodyInstance()->bLockZRotation = false;

	//Door->GetBodyInstance()->bLockRotation = true;

}

void ADoor::Use() {

	//Drop();
	//Item->SetEnableGravity(false);
	//Door->SetActorRotation(FRotator(Door->GetActorRotation().Pitch, 45.0f, Door->GetActorRotation().Roll));
	//Item->SetWorldRotation(FRotator(Item->GetComponentRotation().Pitch, 45.0f, Item->GetComponentRotation().Roll));
	
	//Door->GetBodyInstance()->bLockRotation = false;
	//Door->SetWorldRotation(FRotator(0.0f, 0.0f, 270.0f), true, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);
}

void ADoor::Open() {
	Opened = true;
	Moving = true;
	Item->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f));
	Item->SetPhysicsAngularVelocity(FVector(0.0f, 0.0f, 0.0f));
	//DoorRotation = 0.0f;
}

void ADoor::Close() {
	Opened = false;
	Moving = true;
	Item->SetWorldRotation(FRotator(0.0f, 0.0f, 90.0f));
	//DoorRotation = 0.0f;
}

void ADoor::UpdateRotation(FRotator DeviceRotation) {

	MotionTracker->SetWorldRotation(MotionController->GetComponentRotation() + RootComponent->GetComponentRotation() - FRotator(0.0f, DeviceRotation.Yaw, 0.0f));

	FRotator NewRotator = FRotator(0.0f, 0.0f, 0.0f);

	//If someone's bored, make these use the ? operator
	//if (!Item->GetBodyInstance()->bLockXRotation) {
	//	NewRotator.Pitch = MotionTracker->GetComponentRotation().Pitch;
	//}
	//else {
	//	NewRotator.Pitch = Item->GetComponentRotation().Pitch;
	//}

	//if (!Item->GetBodyInstance()->bLockYRotation) {
	//	NewRotator.Yaw = MotionTracker->GetComponentRotation().Yaw;
	//}
	//else {
	//	NewRotator.Yaw = Item->GetComponentRotation().Yaw;
	//}

	//if (!Item->GetBodyInstance()->bLockZRotation) {
	//	NewRotator.Roll = MotionTracker->GetComponentRotation().Pitch * -1.0f;// +FGenericPlatformMath::Abs(((NewLocation - OldLocation) * 20.0f).Y);
	//}
	//else {
	//	NewRotator.Roll = Item->GetComponentRotation().Pitch;
	//}

	float YLength = FGenericPlatformMath::Abs(MotionTracker->GetComponentLocation().Y - Item->GetComponentLocation().Y);
	float ZLength = FGenericPlatformMath::Abs(MotionTracker->GetComponentLocation().Z - Item->GetComponentLocation().Z);

	UE_LOG(LogTemp, Warning, TEXT("YLen: %f"), YLength);
	UE_LOG(LogTemp, Warning, TEXT("ZLen: %f"), ZLength);
	UE_LOG(LogTemp, Warning, TEXT("Theta: %f"), FMath::RadiansToDegrees(FGenericPlatformMath::Atan(ZLength / YLength)));


	NewRotator.Roll = FMath::RadiansToDegrees(FGenericPlatformMath::Atan(ZLength / YLength));

	/*while (NewRotator.Roll < 0.0f) {
		NewRotator.Roll += 360.0f;
	}

	while (NewRotator.Roll > 360.0f) {
		NewRotator.Roll -= 360.0f;
	}*/

	/*if (NewRotator.Roll > 90.0f) {
		NewRotator.Roll = 90.0f;
	} else if (NewRotator.Roll < 0.0f) {
		NewRotator.Roll = 0.0f;
	}*/

	bool Sign = NewRotator.Roll - Item->GetComponentRotation().Roll > 0.0f;

	if (FGenericPlatformMath::Abs(NewRotator.Roll - Item->GetComponentRotation().Roll) > 1.0f) {
		if (Sign) {
			NewRotator.Roll = Item->GetComponentRotation().Roll + 1.0f;
		} else {
			NewRotator.Roll = Item->GetComponentRotation().Roll - 1.0f;
		}
	}

	Item->SetWorldRotation(NewRotator, true, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);

}

void ADoor::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);

	if (Held) {
		if (!Opened) {
			if (Item->GetComponentRotation().Roll < 5.0f) {
				Open();
				SelfDrop();
			}
		}
		else if (Opened) {
			if (Item->GetComponentRotation().Roll > 85.0f) {
				Close();
				SelfDrop();
			}
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("DOOR_IS_NOT_OPEN_OR_CLOSED"));
		}
	}

	if (Item->GetComponentRotation().Roll > 90.0f) {
		Item->SetWorldRotation(FRotator(0.0f, 0.0f, 90.0f));
	} else if (Item->GetComponentRotation().Roll < 0.0f) {
		Item->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f));
	}

	if (Moving) {
		if (Opened) {
			DoorRotation++;
		} else if (!Opened) {
			DoorRotation--;
		}
		Door->SetActorRotation(FRotator(Door->GetActorRotation().Pitch, DoorRotation, Door->GetActorRotation().Roll));
		if (DoorRotation >= 90.0f || DoorRotation <= -0.0f) {
			Moving = false;
		}
	}

}