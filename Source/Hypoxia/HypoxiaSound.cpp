// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "HypoxiaSound.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationPath.h"

void UHypoxiaSound::PlaySoundAtLocationRefract(const UObject* WorldContextObject, USoundBase* Sound, FVector Location, float VolumeMultiplier, float PitchMultiplier, float StartTime, USoundAttenuation* Attenuation) {
	
	UGameplayStatics::PlaySoundAtLocation(WorldContextObject, Sound, Location, VolumeMultiplier, PitchMultiplier, StartTime, Attenuation);
}

void UHypoxiaSound::PathSoundTo(UObject* WorldContextObject, FVector StartLoc, FVector EndLoc, FVector& Loc_Out) {
	FVector ProjectedLocation;
	UNavigationSystem* NavSys = UNavigationSystem::GetCurrent(WorldContextObject);
	FNavPathSharedPtr Path = NavSys->FindPathToLocationSynchronously(WorldContextObject, StartLoc, EndLoc)->GetPath();
	if (!Path.IsValid())
		return;
	TArray<FNavPathPoint> PathPoints = Path->GetPathPoints();
	FVector Projection = PathPoints[PathPoints.Num() - 2].Location - PathPoints[PathPoints.Num() - 1].Location;
	Projection.Normalize();
	// Projects sound location according to distance original sound would have traveled
	ProjectedLocation = EndLoc + Projection * Path->GetLength();
	UE_LOG(LogTemp, Warning, TEXT("Sound at = %f, %f, %f"), Projection.X, Projection.Y, Projection.Z);
	Loc_Out = ProjectedLocation;
}


