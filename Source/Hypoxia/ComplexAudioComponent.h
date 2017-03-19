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

	UPROPERTY(EditAnywhere, Category = "Sound|Complex")
	bool bAdvancedOcclusion;
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f), Category = "Sound|Complex")
	float Radius;

	UPROPERTY(EditAnywhere, Category = "Sound|Complex")
	bool bListenToSelf;

	UPROPERTY(EditAnywhere, Category = "Sound|Complex")
	bool bAmbientSound;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f, ClampMax = 100.f), Category = "Sound|Complex")
	float ProjectedVolume;

private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* VirtualAudioComponent;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	USphereComponent* InfluenceSphereAudio;

	float TestOcclusion();
	void DiffractSound(FVector goalLoc, FVector& out_Loc, float& out_Vol);
};
