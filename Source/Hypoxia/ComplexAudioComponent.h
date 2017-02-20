// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/AudioComponent.h"
#include "ComplexAudioComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Audio), hidecategories = (Object, ActorComponent, Physics, Rendering, Mobility, LOD), ShowCategories = Trigger, meta = (BlueprintSpawnableComponent))
class HYPOXIA_API UComplexAudioComponent : public UAudioComponent
{
	GENERATED_BODY()
	
public:
	UComplexAudioComponent();
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float, ELevelTick, FActorComponentTickFunction*) override;
	void DestroyComponent(bool bPromoteChildren = false);
	void Play(float startTime = 0.f);
	void Stop();
	void SetSound(USoundBase*);
	void SetAttenuationSettings(USoundAttenuation*);

	UPROPERTY(EditAnywhere)
	bool bAdvancedOcclusion;
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f))
	float Radius;

	UPROPERTY(EditAnywhere)
	bool bListenToSelf;

private:
	UAudioComponent* VirtualAudioComponent;
	USphereComponent* InfluenceSphere;

	float TestOcclusion();
	void DiffractSound(FVector goalLoc, FVector& out_Loc, float& out_Vol);
};
