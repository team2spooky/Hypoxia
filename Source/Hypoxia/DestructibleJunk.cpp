// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "DestructibleJunk.h"

ADestructibleJunk::ADestructibleJunk() {
	DestructibleMesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Destructible"));
	DestructibleMesh->SetupAttachment(Item);
}

void ADestructibleJunk::DoHit(FVector ImpactPoint, FVector NormalImpulse) {
	DestructibleMesh->SetCollisionProfileName(FName("Item"));
	DestructibleMesh->ApplyDamage(100.f, ImpactPoint, NormalImpulse, NormalImpulse.Size() / 100);
}

