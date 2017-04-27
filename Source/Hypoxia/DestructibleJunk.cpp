// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "DestructibleJunk.h"

ADestructibleJunk::ADestructibleJunk() {
	DestructibleMesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Destructible"));
	DestructibleMesh->SetupAttachment(Item);
}

void ADestructibleJunk::DoHit(FVector ImpactPoint, FVector NormalImpulse) {
	if (!Held) {
		DestructibleMesh->DetachFromParent(true);
		DestructibleMesh->SetCollisionProfileName(FName("Destructible"));
		DestructibleMesh->ApplyDamage(100.f, ImpactPoint, NormalImpulse * FMath::VRandCone(NormalImpulse, PI / 16), NormalImpulse.Size() / 100);
		Broken = true;
	}
}


bool ADestructibleJunk::Pickup(USceneComponent*, EControllerHand) {
}
