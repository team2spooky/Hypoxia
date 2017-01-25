// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "HypoxiaAIController.h"

void AHypoxiaAIController::TrackPlayer(float volume) {
	auto playerLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
	auto vector = new FVector(FMath::Lerp(400, 50, volume/100), 0.0f, 0.0f);
	vector->RotateAngleAxis(FMath::Lerp(0, 359, FMath::Rand() / RAND_MAX), vector->UpVector);
	MoveToLocation(playerLocation + *vector, 5.0f);

	UE_LOG(LogTemp, Warning, TEXT("Chase!"));
}


