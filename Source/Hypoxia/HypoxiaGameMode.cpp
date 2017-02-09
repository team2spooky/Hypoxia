// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Hypoxia.h"
#include "HypoxiaGameMode.h"
#include "HypoxiaCharacter.h"
#include "HypoxiaMonster.h"
#include "HypoxiaAIController.h"
#include "HypoxiaSound.h"
#include <EngineGlobals.h>
#include "Runtime/Online/Voice/Public/Voice.h"
#include "Sound/SoundWaveProcedural.h"
#include "Sound/SoundCue.h"

TSharedPtr<IVoiceCapture> VoiceCapture;
TArray<uint8> VoiceCaptureBuffer;

// voice playback
/*
UAudioComponent* VoiceCaptureAudioComponent;
USoundWaveProcedural* VoiceCaptureSoundWaveProcedural;
bool PlayVoiceCaptureFlag;
FTimerHandle PlayVoiceCaptureTimer;
*/
USoundCue* testSound;
USoundAttenuation* testAttenuation;

AHypoxiaGameMode::AHypoxiaGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/HypoxiaCPP/Blueprints/HypoxiaCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class

	//HUDClass = AHypoxiaHUD::StaticClass();

	// Turns on GameMode ticking
	PrimaryActorTick.bCanEverTick = true;

	// initializes and starts voice capture
	VoiceCapture = FVoiceModule::Get().CreateVoiceCapture();
	if(VoiceCapture.IsValid())
		VoiceCapture->Start();

	// creates voice capture component
	/*
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

	static ConstructorHelpers::FObjectFinder<USoundCue> test(TEXT("'/Game/StarterContent/Audio/Explosion_Cue.Explosion_Cue'"));
	testSound = test.Object;
	static ConstructorHelpers::FObjectFinder<USoundAttenuation> test2(TEXT("'/Game/TestAttenuation.TestAttenuation'"));
	testAttenuation = test2.Object;
}

void AHypoxiaGameMode::BeginPlay() {
	Super::BeginPlay();
}

void AHypoxiaGameMode::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

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
		VoiceCaptureFinalVolume = ((VoiceCaptureRms / 32768.0) * 200.f);

		//VoiceCaptureSoundWaveProcedural->QueueAudio(VoiceCaptureBuffer.GetData(), VoiceCaptureReadBytes);
		//VoiceCaptureAudioComponent->SetSound(VoiceCaptureSoundWaveProcedural);

		//PlayVoiceCaptureFlag = true;

		UE_LOG(LogTemp, Warning, TEXT("Volume = %f"), VoiceCaptureFinalVolume);

		if (VoiceCaptureFinalVolume > 10) {
			TArray<AActor *> monsters;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHypoxiaMonster::StaticClass(), monsters);
			for (AActor * monster : monsters) {
				//Cast<AHypoxiaAIController>(Cast<APawn>(monster)->Controller)->TrackPlayer(VoiceCaptureFinalVolume);
				FVector Loc;
				UHypoxiaSound::PathSoundTo(this, monster->GetActorLocation(), UGameplayStatics::GetPlayerCharacter(this, 0)->GetActorLocation(), Loc);
				UHypoxiaSound::PlaySoundAtLocationRefract(this, testSound, Loc, 1.0f, 1.0f, 0.0f, testAttenuation);
			}
		}
	}
}
