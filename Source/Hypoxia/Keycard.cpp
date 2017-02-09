// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "Keycard.h"
#include "EngineUtils.h"
#include "Engine/StaticMeshActor.h"


AStaticMeshActor *Door;



void AKeycard::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);

	if (FVector::Dist(Door->GetActorLocation(), Item->GetComponentLocation()) < 1000.0f) {
		//UnlockLock();
		Door->SetActorLocation(FVector(800.0f, 100.0f, 400.0f));
	}

}



void AKeycard::BeginPlay() {

	Super::BeginPlay();

	for (TActorIterator<AStaticMeshActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr->ActorHasTag(FName("DDoor"))) {

			Door = *ActorItr;

			UE_LOG(LogTemp, Warning, TEXT("Parent: %s"), *ActorItr->GetName());
			UE_LOG(LogTemp, Warning, TEXT("Im done"));
			break;
		}
	}
}

void AKeycard::UnlockLock()
{
	Door->SetActorLocation(FVector(800.0f,100.0f,140.0f));
}
