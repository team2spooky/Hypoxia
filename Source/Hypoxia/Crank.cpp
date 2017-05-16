// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "Crank.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "DrawDebugHelpers.h"

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
		FVector  DevicePosition;
		FRotator DeviceRotation;

		UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(DeviceRotation, DevicePosition);

		MotionTracker->SetWorldLocation(MotionController->GetComponentLocation() + RootComponent->GetComponentLocation() - DevicePosition + FVector(0.0f, 0.0f, DevicePosition.Z - 100.f), false, (FHitResult*)nullptr, ETeleportType::None);
		

		DrawDebugLine(GetWorld(), Base->GetComponentLocation(), Base->GetComponentLocation() + Base->GetRightVector() * 100.f, FColor::Red, false, -1.0f, (uint8)'\000', 0.1f);
		DrawDebugLine(GetWorld(), Base->GetComponentLocation(), Base->GetComponentLocation() + Base->GetForwardVector() * 100.f, FColor::Green, false, -1.0f, (uint8)'\000', 0.1f);
		DrawDebugLine(GetWorld(), Base->GetComponentLocation(), Base->GetComponentLocation() + Base->GetUpVector() * 100.f, FColor::Blue, false, -1.0f, (uint8)'\000', 0.1f);

		FVector Direction = Constraint->GetComponentLocation() - MotionTracker->GetComponentLocation();
		DrawDebugLine(GetWorld(), Constraint->GetComponentLocation(), MotionTracker->GetComponentLocation(), FColor::Purple, false, -1.0f, (uint8)'\000', 0.1f);
		Direction = FVector::VectorPlaneProject(Direction, Base->GetRightVector());
		Direction = Direction.GetSafeNormal();
		Direction *= -1.f;
		DrawDebugLine(GetWorld(), Constraint->GetComponentLocation(), Constraint->GetComponentLocation() + Direction * 50.f, FColor::Purple, false, -1.0f, (uint8)'\000', 0.1f);

		float dot = Direction | Base->GetForwardVector();
		float det = FMatrix(Direction, Base->GetForwardVector(), Base->GetRightVector(), FVector::ZeroVector).RotDeterminant();
		Angle = FMath::Atan2(det, dot);
		UE_LOG(LogTemp, Warning, TEXT("%f"), FMath::RadiansToDegrees(Angle));

		//Item->SetWorldRotation(FQuat(Base->GetRightVector() * -1.f, TestAngle).Rotator());
		Constraint->SetAngularOrientationTarget(FRotator(-1 * FMath::RadiansToDegrees(Angle), 0.f, 0.f));

		//Fallback
		/*Item->SetWorldRotation(Item->GetComponentRotation().Add(0.f, 0.f, 10.f), true);
		Angle += 10.f;*/

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

