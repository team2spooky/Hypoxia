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
	Constraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 45.0);
	Constraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45.0);
}

void ACrank::BeginPlay() {
	Super::BeginPlay();

	Base->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	/*if (bOn) {
		Constraint->SetAngularOrientationTarget(FRotator(-1 * MaxAngle, 0.f, 0.f));
		Rev = MaxAngle / 360.0f;
		Angle = FMath::DegreesToRadians(FMath::Fmod(MaxAngle, 360.0f));
	} else {
		Constraint->SetAngularOrientationTarget(FRotator(-1 * MinAngle, 0.f, 0.f));
		Rev = MinAngle / 360.0f;
		Angle = FMath::DegreesToRadians(FMath::Fmod(MinAngle, 360.0f));
	}*/
}

void ACrank::Drop() {
	Super::Drop();
	bool prevOn = bOn;
	if (AngleTraveled + AngleTolerance >= MaxAngle) {
		//Constraint->SetAngularOrientationTarget(FRotator(-1 * MaxAngle, 0.f, 0.f));
		bOn = true;
	} else if (AngleTraveled - AngleTolerance <= MinAngle) {
		//Constraint->SetAngularOrientationTarget(FRotator(-1 * MinAngle, 0.f, 0.f));
		bOn = false;
	} else {
		return;
	}
	if (prevOn != bOn) {
		if (bOn) {
			EventOn();
		}
		else {
			EventOff();
		}
	}
}

void ACrank::Tick(float DeltaTime) {
	if (Held) {
		FVector  DevicePosition;
		FRotator DeviceRotation;

		UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(DeviceRotation, DevicePosition);

		MotionTracker->SetWorldLocation(MotionController->GetComponentLocation() + RootComponent->GetComponentLocation() - DevicePosition + FVector(0.0f, 0.0f, DevicePosition.Z - 100.f), false, (FHitResult*)nullptr, ETeleportType::None);

		float prevAngle = Angle;

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
		float CalculatedAngle = -1 * FMath::Atan2(det, dot);

		float DeltaAngle = CalculatedAngle - prevAngle;
		if (DeltaAngle <= -1 * PI) DeltaAngle += 2 * PI;
		else if (DeltaAngle > PI) DeltaAngle -= 2 * PI;

		AngleTraveled += FMath::RadiansToDegrees(DeltaAngle);
		float NormalAngle = NormalizeAngle(CalculatedAngle);
		if (AngleTraveled >= MaxAngle) {
			Constraint->SetAngularOrientationTarget(FRotator(-1 * MaxAngle, 0.f, 0.f));
			AngleTraveled = MaxAngle;
		} else if (AngleTraveled <= MinAngle) {
			Constraint->SetAngularOrientationTarget(FRotator(-1 * MinAngle, 0.f, 0.f));

			AngleTraveled = MinAngle;
		} else {
			Constraint->SetAngularOrientationTarget(FRotator(FMath::RadiansToDegrees(CalculatedAngle), 0.f, 0.f));
			Angle = CalculatedAngle;
		}

		UE_LOG(LogTemp, Warning, TEXT("Angle: %f, Delta Angle: %f, Traveled Angle: %f"), NormalizeAngle(Angle), DeltaAngle, AngleTraveled);

		//Fallback
		/*Item->SetWorldRotation(Item->GetComponentRotation().Add(0.f, 0.f, 10.f), true);
		Angle += 10.f;*/

		if (FVector::Dist(MotionTracker->GetComponentLocation(), GrabSpot->GetComponentLocation()) > 30.0f) {
			SelfDrop();
		}
	}
}

float ACrank::NormalizeAngle(float Theta) {
	return Theta + (Theta < 0 ? 2 * PI : 0);
}

void ACrank::EventOn_Implementation() {
}

void ACrank::EventOff_Implementation() {
}

