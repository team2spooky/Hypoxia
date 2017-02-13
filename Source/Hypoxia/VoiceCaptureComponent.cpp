// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "ListeningItem.h"
#include "VoiceCaptureComponent.h"


// Sets default values for this component's properties
UVoiceCaptureComponent::UVoiceCaptureComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// initializes and starts voice capture
	VoiceCapture = FVoiceModule::Get().CreateVoiceCapture();

	/*
	// creates voice capture playback component
	VoiceCaptureAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("VoiceCaptureAudioComponent"));
	VoiceCaptureAudioComponent->SetupAttachment(RootComponent);
	VoiceCaptureAudioComponent->bAutoActivate = true;
	VoiceCaptureAudioComponent->bAlwaysPlay = true;
	VoiceCaptureAudioComponent->VolumeMultiplier = 5.f;

	// creates voice capture sound wave
	VoiceCaptureSoundWaveProcedural = NewObject<USoundWaveProcedural>();
	VoiceCaptureSoundWaveProcedural->SampleRate = 16000;
	VoiceCaptureSoundWaveProcedural->NumChannels = 1;
	VoiceCaptureSoundWaveProcedural->Duration = INDEFINITELY_LOOPING_DURATION;
	VoiceCaptureSoundWaveProcedural->SoundGroup = SOUNDGROUP_Voice;
	VoiceCaptureSoundWaveProcedural->bLooping = false;
	VoiceCaptureSoundWaveProcedural->bProcedural = true;
	VoiceCaptureSoundWaveProcedural->Volume = 5.f;
	*/

	InfluenceSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Influence Sphere"));
	InfluenceSphere->SetSphereRadius(200.0f);
	InfluenceSphere->bHiddenInGame = false;
	InfluenceSphere->SetRelativeLocation(FVector(0.0f));
}


// Called when the game starts
void UVoiceCaptureComponent::BeginPlay()
{
	Super::BeginPlay();

	InfluenceSphere->SetRelativeLocation(FVector(0.0f));
	if (VoiceCapture.IsValid())
		VoiceCapture->Start();
	else
		PrimaryComponentTick.bCanEverTick = false;
}


// Called every frame
void UVoiceCaptureComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (!VoiceCapture.IsValid())
		return;

	uint32 VoiceCaptureBytesAvailable = 0;
	EVoiceCaptureState::Type CaptureState = VoiceCapture->GetCaptureState(VoiceCaptureBytesAvailable);

	//PlayVoiceCaptureFlag = false;

	VoiceCaptureBuffer.Reset();

	if (CaptureState == EVoiceCaptureState::Ok && VoiceCaptureBytesAvailable > 0) {
		int16_t VoiceCaptureSample;
		uint32 VoiceCaptureReadBytes;
		float VoiceCaptureTotalSquared = 0;

		VoiceCaptureBuffer.SetNumUninitialized(VoiceCaptureBytesAvailable);

		VoiceCapture->GetVoiceData(VoiceCaptureBuffer.GetData(), VoiceCaptureBytesAvailable, VoiceCaptureReadBytes);

		for (uint32 i = 0; i < (VoiceCaptureReadBytes / 2); i++)
		{
			VoiceCaptureSample = (VoiceCaptureBuffer[i * 2 + 1] << 8) | VoiceCaptureBuffer[i * 2];
			VoiceCaptureTotalSquared += ((float)VoiceCaptureSample * (float)VoiceCaptureSample);
		}

		float VoiceCaptureMeanSquare = (2 * (VoiceCaptureTotalSquared / VoiceCaptureBuffer.Num()));
		float VoiceCaptureRms = FMath::Sqrt(VoiceCaptureMeanSquare);
		float VoiceCaptureFinalVolume = ((VoiceCaptureRms / 32768.0) * 200.f);

		//VoiceCaptureSoundWaveProcedural->QueueAudio(VoiceCaptureBuffer.GetData(), VoiceCaptureReadBytes);
		//VoiceCaptureAudioComponent->SetSound(VoiceCaptureSoundWaveProcedural);
		//PlayVoiceCaptureFlag = true;

		//UE_LOG(LogTemp, Warning, TEXT("Volume = %f"), VoiceCaptureFinalVolume);

		if (VoiceCaptureFinalVolume > 10) {
			TriggerObjects(VoiceCaptureFinalVolume);
		}
	}
}

void UVoiceCaptureComponent::TriggerObjects(float volume) {
	InfluenceSphere->SetSphereRadius(FMath::Lerp(0, 2000, volume / 100.0f));
	TSet<AActor*> OverlappingActors;
	TSubclassOf<AListeningItem> Filter = AListeningItem::StaticClass();
	InfluenceSphere->GetOverlappingActors(OverlappingActors, Filter);
	for (TSet<AActor*>::TConstIterator Itr = OverlappingActors.CreateConstIterator(); Itr; ++Itr) {
		Cast<AListeningItem>(*Itr)->Hear(volume);
	}
}

