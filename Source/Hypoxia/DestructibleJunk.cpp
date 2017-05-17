// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "DestructibleJunk.h"

ADestructibleJunk::ADestructibleJunk() {
	DestructibleMesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Destructible"));
	DestructibleMesh->SetupAttachment(Item);

	Item->SetVisibility(false);
}

void ADestructibleJunk::DoHit(FVector ImpactPoint, FVector NormalImpulse) {
	if (!Held && !Broken) {
		DestructibleMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DestructibleMesh->ApplyDamage(10.f, DestructibleMesh->GetComponentLocation(), NormalImpulse, NormalImpulse.Size() / 50);
		Broken = true;
	}
}

bool ADestructibleJunk::Pickup(USceneComponent* SceneComponent, EControllerHand ControllerHand) {
	if (Broken) return false;
	return Super::Pickup(SceneComponent, ControllerHand);
}

void ADestructibleJunk::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) {
	if ((Item->GetPhysicsLinearVelocity() - LastVelocity).Size() > 150 && !Broken) {
		LastVelocity = Item->GetPhysicsLinearVelocity();
		if (OnHitSound) {
			UComplexAudioComponent* HitSound = GenerateOnHitSound();
			HitSound->Play();
		}
		DoHit(Hit.ImpactPoint, NormalImpulse);
	}
}
