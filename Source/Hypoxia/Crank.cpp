// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "Crank.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"

ACrank::ACrank() {
	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	Base->SetupAttachment(Item_Base);
	Base->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	Constraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Constraint"));
	Constraint->SetupAttachment(Base);
	Constraint->bAutoActivate = true;
	Constraint->SetConstrainedComponents(this->Base, NAME_None, this->Item, NAME_None);
	Constraint->InitComponentConstraint();

	Constraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45.0);
	Constraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 45.0);
	Constraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45.0);
}

void ACrank::BeginPlay() {
	Super::BeginPlay();

	Base->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	/*if (bOn) {
		Constraint->SetLinearPositionTarget(FVector(0.f, 0.f, -1 * TravelDistance));
	}
	else {
		Constraint->SetLinearPositionTarget(FVector(0.f, 0.f, TravelDistance));
	}*/
}

void ACrank::Drop() {
	Super::Drop();
	/*FVector A = Item->GetComponentLocation() - Constraint->GetComponentLocation();
	float Direction = FVector::DotProduct(A, Constraint->GetUpVector());
	Constraint->SetLinearPositionTarget(FVector(0.f, 0.f, -1 * FMath::Sign(Direction) * TravelDistance));
	bool temp = bOn;
	bOn = Direction > 0;
	if (temp == bOn) return;
	if (bOn) {
		EventOn();
	}
	else {
		EventOff();
	}*/
}

void ACrank::Tick(float DeltaTime) {
	if (Held) {
		/*FVector  DevicePosition;
		FRotator DeviceRotation;

		UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(DeviceRotation, DevicePosition);

		MotionTracker->SetWorldLocation(MotionController->GetComponentLocation() + RootComponent->GetComponentLocation() - DevicePosition + FVector(0.0f, 0.0f, DevicePosition.Z - 100.f), false, (FHitResult*)nullptr, ETeleportType::None);
		*/
		/*FVector Direction = Constraint->GetComponentLocation() - MotionTracker->GetComponentLocation();
		Direction = FVector::VectorPlaneProject(Direction, Item->GetRightVector());
		Direction.Normalize();
		float Angle = (FVector::DotProduct(Direction, Item->GetForwardVector()));
		UE_LOG(LogTemp, Warning, TEXT("%f"), Angle / PI * 180.f);*/

		//Item->SetWorldRotation(Direction.Rotation(), true);
		
		//Item->SetRelativeRotation(FRotator::MakeFromEuler(FVector(0.f, Angle / PI * 180.f, 0.f)), true);

		//Works Kinda
		//Item->SetWorldRotation(FRotationMatrix::MakeFromXY(Constraint->GetComponentLocation() - MotionTracker->GetComponentLocation(), Item->GetRightVector()).Rotator(), true);

		Item->SetWorldRotation(Item->GetComponentRotation().Add(0.f, 0.f, 10.f), true);
		Angle += 10.f;

		if (FVector::Dist(MotionTracker->GetComponentLocation(), GrabSpot->GetComponentLocation()) > 250.0f) {
			SelfDrop();
		}
	}
	/*
	if(Angle >= 1080.f) {
		EventOn();
	}*/
}

void ACrank::EventOn_Implementation() {
}

void ACrank::EventOff_Implementation() {
}

