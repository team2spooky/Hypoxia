// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "SlidingSwitch.h"

ASlidingSwitch::ASlidingSwitch() {
	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	Base->SetupAttachment(Item_Base);
	Base->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	Constraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Constraint"));
	Constraint->SetupAttachment(Base);
	Constraint->bAutoActivate = true;
	Constraint->SetConstrainedComponents(this->Base, NAME_None, this->Item, NAME_None);
	Constraint->InitComponentConstraint();

	Constraint->SetLinearZLimit(ELinearConstraintMotion::LCM_Limited, TravelDistance);
	Constraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45.0);
	Constraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 45.0);
	Constraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45.0);
	Constraint->SetLinearPositionDrive(false, false, true);
	Constraint->SetLinearDriveParams(500.f, 500.f, 0.f);

	Item->GetBodyInstance()->bLockRotation = true;
}

void ASlidingSwitch::BeginPlay() {
	Super::BeginPlay();

	Base->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	if (bOn) {
		Constraint->SetLinearPositionTarget(FVector(0.f, 0.f, -1 * TravelDistance));
	} else {
		Constraint->SetLinearPositionTarget(FVector(0.f, 0.f, TravelDistance));
	}
}

void ASlidingSwitch::Drop() {
	Super::Drop();
	FVector A = Item->GetComponentLocation() - Constraint->GetComponentLocation();
	float Direction = FVector::DotProduct(A, Constraint->GetUpVector());
	Constraint->SetLinearPositionTarget(FVector(0.f, 0.f, -1 * FMath::Sign(Direction) * TravelDistance));
	bool temp = bOn;
	bOn = Direction > 0;
	if (temp == bOn) return;
	if (bOn) {
		SwitchOn();
		OnSwitchOn.Broadcast();
	} else {
		SwitchOff();
		OnSwitchOff.Broadcast();
	}
}

void ASlidingSwitch::SwitchOn_Implementation() {
}

void ASlidingSwitch::SwitchOff_Implementation() {
}
