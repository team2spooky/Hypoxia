// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "Keycard.h"
#include "EngineUtils.h"
#include "Engine/StaticMeshActor.h"


void AKeycard::BeginPlay() {

	Super::BeginPlay();

	for (TActorIterator<ADoor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr->ActorHasTag(Tags[0])) {

			Door = *ActorItr;
			UE_LOG(LogTemp, Warning, TEXT("Child: %s"), *ActorItr->GetName());
			UE_LOG(LogTemp, Warning, TEXT("Parent: %s"), *GetName());
			break;
		}
	}
}

void AKeycard::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);

	if (Held) {
		if (FVector::Dist(Door->GetActorLocation(), Item->GetComponentLocation()) < 50.0f) {
			Door->Unlock();
			UE_LOG(LogTemp, Warning, TEXT("Unlocking"));
		}
	}

}

