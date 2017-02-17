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

	//for (TActorIterator<AStaticMeshActor> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
	//	
	//	AStaticMeshActor *Mesh = *ActorItr;

	//	//UE_LOG(LogTemp, Warning, TEXT("Parent: %s"), ActorItr->Tags[0]);

	//	//FName Tag = FName()

	//	if (ActorItr->ActorHasTag(Tags[0])) {
	//		
	//		Door = *ActorItr;
	//		
	//		UE_LOG(LogTemp, Warning, TEXT("Child: %s"), *ActorItr->GetName());
	//		UE_LOG(LogTemp, Warning, TEXT("Parent: %s"),  *GetName());
	//		break;
	//	}
	//}

	//Moving = false;
	Opened = false;

	DoorRotation = 0.0f;

	InitialPosition = Item->GetComponentLocation();

	//Item->SetEnableGravity(false);
	//Item->GetBodyInstance()->bLockTranslation = true;
	Item->GetBodyInstance()->bLockRotation = true;
	//Item->GetBodyInstance()->bLockXRotation = true;
	//Item->GetBodyInstance()->bLockYRotation = true;
	//Item->GetBodyInstance()->bLockZRotation = false;

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
	//Moving = true;
	//Item->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f));
	//Item->SetPhysicsAngularVelocity(FVector(0.0f, 0.0f, 0.0f));
	//DoorRotation = 0.0f;
}

void ADoor::Close() {
	Opened = false;
	//Moving = true;
	/*if (DoorHandleDirection == EDoorDirection::DD_North || DoorHandleDirection == EDoorDirection::DD_South) {
		Item->SetWorldRotation(FRotator(0.0f, 0.0f, 90.0f));
	} else if (DoorHandleDirection == EDoorDirection::DD_East || DoorHandleDirection == EDoorDirection::DD_West) {
		Item->SetWorldRotation(FRotator(0.0f, 90.0f, 0.0f));
	} else {
		UE_LOG(LogTemp, Error, TEXT("DOOR_DIRECTION_NOT_PROPERLY_DEFINED"));
	}*/
	//Item->SetWorldRotation(FRotator(0.0f, 0.0f, 90.0f));
	//DoorRotation = 0.0f;
}

//void ADoor::UpdateRotation(FRotator DeviceRotation) {

	//MotionTracker->SetWorldRotation(MotionController->GetComponentRotation() + RootComponent->GetComponentRotation() - FRotator(0.0f, DeviceRotation.Yaw, 0.0f));

	//FRotator NewRotator = FRotator(0.0f, 0.0f, 0.0f);

	//float XLength = FMath::Abs(MotionTracker->GetComponentLocation().X - Item->GetComponentLocation().X);
	//float YLength = FMath::Abs(MotionTracker->GetComponentLocation().Y - Item->GetComponentLocation().Y);
	//float ZLength = FMath::Abs(MotionTracker->GetComponentLocation().Z - Item->GetComponentLocation().Z);

	//float XLength = MotionTracker->GetComponentLocation().X - Item->GetComponentLocation().X;
	//float YLength = MotionTracker->GetComponentLocation().Y - Item->GetComponentLocation().Y;
	//float ZLength = MotionTracker->GetComponentLocation().Z - Item->GetComponentLocation().Z;

	////UE_LOG(LogTemp, Warning, TEXT("YLen: %f"), YLength);
	////UE_LOG(LogTemp, Warning, TEXT("ZLen: %f"), ZLength);
	////UE_LOG(LogTemp, Warning, TEXT("Theta: %f"), FMath::RadiansToDegrees(FGenericPlatformMath::Atan(ZLength / YLength)));

	//float NewRotation = 0.0f;

	//if (DoorHandleDirection == EDoorDirection::DD_North || DoorHandleDirection == EDoorDirection::DD_South) {
	//	NewRotation = FMath::RadiansToDegrees(FGenericPlatformMath::Atan2(ZLength, YLength));
	//}
	//else if (DoorHandleDirection == EDoorDirection::DD_East || DoorHandleDirection == EDoorDirection::DD_West) {
	//	NewRotation = FMath::RadiansToDegrees(FGenericPlatformMath::Atan2(ZLength, XLength));
	//} else {
	//	UE_LOG(LogTemp, Error, TEXT("DOOR_DIRECTION_NOT_PROPERLY_DEFINED"));
	//}

	//while (NewRotation < 0.0f) {
	//	NewRotation += 360.0f;
	//}

	//while (NewRotation > 360.0f) {
	//	NewRotation -= 360.0f;
	//}

	//if (NewRotation > 90.0f) {
	//	NewRotation = 90.0f;
	//} else if (NewRotation < 0.0f) {
	//	NewRotation = 0.0f;
	//}

	//if (DoorHandleDirection == EDoorDirection::DD_North || DoorHandleDirection == EDoorDirection::DD_East) {
	//	NewRotation = 90.0f - NewRotation;
	//}

	//float ItemRotation = 0.0f;

	//if (DoorHandleDirection == EDoorDirection::DD_North || DoorHandleDirection == EDoorDirection::DD_South) {
	//	ItemRotation = Item->GetComponentRotation().Roll;
	//} else if (DoorHandleDirection == EDoorDirection::DD_East || DoorHandleDirection == EDoorDirection::DD_West) {
	//	ItemRotation = Item->GetComponentRotation().Pitch;
	//} else {
	//	UE_LOG(LogTemp, Error, TEXT("DOOR_DIRECTION_NOT_PROPERLY_DEFINED"));
	//}

	//bool Sign = NewRotation - ItemRotation > 0.0f;
	//
	//if (FGenericPlatformMath::Abs(NewRotation - Item->GetComponentRotation().Roll) > 1.0f) {
	//	if (Sign) {
	//		NewRotation = ItemRotation + 1.0f;
	//	} else {
	//		NewRotation = ItemRotation - 1.0f;
	//	}
	//}

	//if (DoorHandleDirection == EDoorDirection::DD_North || DoorHandleDirection == EDoorDirection::DD_South) {
	//	NewRotator.Roll  = NewRotation;
	//} else if (DoorHandleDirection == EDoorDirection::DD_East || DoorHandleDirection == EDoorDirection::DD_West) {
	//	NewRotator.Pitch = NewRotation;
	//} else {
	//	UE_LOG(LogTemp, Error, TEXT("DOOR_DIRECTION_NOT_PROPERLY_DEFINED"));
	//}

	//UE_LOG(LogTemp, Warning, TEXT("Theta: %f"), NewRotation);

	//Item->SetRelativeRotation(NewRotator, true, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);
//}

void ADoor::UpdatePosition(FVector DevicePosition) {

	MotionTracker->SetWorldLocation(MotionController->GetComponentLocation() + RootComponent->GetComponentLocation() - DevicePosition + FVector(0.0f, 0.0f, DevicePosition.Z - 100.f), false, (FHitResult*)nullptr, ETeleportType::None);

	/*OldLocation = NewLocation;
	NewLocation = MotionTracker->GetComponentLocation();*/

	FVector NextLocation = MotionTracker->GetComponentLocation();

	bool Sign = NextLocation.Y - Item->GetComponentLocation().Y > 0.0f;

	if (FGenericPlatformMath::Abs(NextLocation.Y - Item->GetComponentLocation().Y) > 1.0f) {
		if (Sign) {
			NextLocation.Y = Item->GetComponentLocation().Y + 1.0f;
		} else {
			NextLocation.Y = Item->GetComponentLocation().Y - 1.0f;
		}
	}

	if (DoorHandleDirection == EDoorDirection::DD_East) {
		//Door is not past where it starts and 125 units moved
		if (NextLocation.Y > InitialPosition.Y && NextLocation.Y < InitialPosition.Y + 125.0f) {
			Item->SetWorldLocation(NextLocation, true, (FHitResult*)nullptr, ETeleportType::None);
		}
	} else if (DoorHandleDirection == EDoorDirection::DD_West) {
		//Door is not past where it starts and 125 units moved
		if (NextLocation.Y < InitialPosition.Y && NextLocation.Y > InitialPosition.Y - 125.0f) {
			Item->SetWorldLocation(NextLocation, true, (FHitResult*)nullptr, ETeleportType::None);
		}
	}

}

void ADoor::Tick(float DeltaTime) {

//	if (!Locked) {

	Super::Tick(DeltaTime);

		/*if (DoorHandleDirection == EDoorDirection::DD_North || DoorHandleDirection == EDoorDirection::DD_South) {
			if (Item->GetComponentRotation().Roll > 90.0f) {
				Item->SetWorldRotation(FRotator(0.0f, 0.0f, 90.0f));
			} else if (Item->GetComponentRotation().Roll < 0.0f) {
				Item->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f));
			}
			if (Held) {
				if (!Opened) {
					if (Item->GetComponentRotation().Roll < 5.0f) {
						Open();
						SelfDrop();
					}
				} else if (Opened) {
					if (Item->GetComponentRotation().Roll > 85.0f) {
						Close();
						SelfDrop();
					}
				} else {
					UE_LOG(LogTemp, Error, TEXT("DOOR_IS_NOT_OPEN_OR_CLOSED"));
				}
			}
		} else if (DoorHandleDirection == EDoorDirection::DD_East || DoorHandleDirection == EDoorDirection::DD_West) {
			if (Item->GetComponentRotation().Pitch > 90.0f) {
				Item->SetWorldRotation(FRotator(0.0f, 90.0f, 0.0f));
			} else if (Item->GetComponentRotation().Pitch < 0.0f) {
				Item->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f));
			}
			if (Held) {
				if (!Opened) {
					if (Item->GetComponentRotation().Pitch < 5.0f) {
						Open();
						SelfDrop();
					}
				} else if (Opened) {
					if (Item->GetComponentRotation().Pitch > 85.0f) {
						Close();
						SelfDrop();
					}
				} else {
					UE_LOG(LogTemp, Error, TEXT("DOOR_IS_NOT_OPEN_OR_CLOSED"));
				}
			}
		} else {
			UE_LOG(LogTemp, Error, TEXT("DOOR_DIRECTION_NOT_PROPERLY_DEFINED"));
		}

		if (Moving) {
			if (Opened) {
				DoorRotation += 0.3f;
			} else if (!Opened) {
				DoorRotation -= 0.3f;
			}
			Door->SetActorRotation(FRotator(Door->GetActorRotation().Pitch, DoorRotation, Door->GetActorRotation().Roll));
			if (DoorRotation >= 90.0f || DoorRotation <= -0.0f) {
				Moving = false;
			}
		}
	}*/

}

void ADoor::Unlock() {
	UE_LOG(LogTemp, Warning, TEXT("Unlocked"));
	Locked = false;
	UE_LOG(LogTemp, Warning, TEXT("Locked? %d"), Locked);
}