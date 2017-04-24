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

	Constraint->SetLinearYLimit(ELinearConstraintMotion::LCM_Limited, TravelDistance);
	Constraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45.0);
	Constraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 45.0);
	Constraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45.0);

	Item->GetBodyInstance()->bLockRotation = true;
}

void ASlidingSwitch::BeginPlay() {
	Super::BeginPlay();

	Base->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	Drop();
}

void ASlidingSwitch::Drop() {
	Super::Drop();
	float Direction = Item->GetComponentLocation().Y - Constraint->GetComponentLocation().Y;
	Item->SetWorldLocation(Item->GetComponentLocation() + FVector(0.f, FMath::Sign(Direction) * TravelDistance, 0.f), true);
}
