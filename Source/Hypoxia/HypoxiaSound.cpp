// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "HypoxiaSound.h"
#include "HypoxiaMonster.h"
#include "HypoxiaAIController.h"
#include "HypoxiaSound.h"
#include <EngineGlobals.h>
#include "Runtime/Online/Voice/Public/Voice.h"
#include "Sound/SoundWaveProcedural.h"
#include "Sound/SoundCue.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationPath.h"

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

void UHypoxiaSound::InitializeVoiceCapture() {
	// initializes and starts voice capture
	VoiceCapture = FVoiceModule::Get().CreateVoiceCapture();
	if (VoiceCapture.IsValid())
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

	//test stuff
	static ConstructorHelpers::FObjectFinder<USoundCue> test(TEXT("'/Game/StarterContent/Audio/Explosion_Cue'"));
	testSound = test.Object;
	static ConstructorHelpers::FObjectFinder<USoundAttenuation> test2(TEXT("'/Game/TestAttenuation'"));
	testAttenuation = test2.Object;
}

void UHypoxiaSound::CaptureVoice(UObject* WorldContextObject) {
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

		UE_LOG(LogTemp, Warning, TEXT("Volume = %f"), VoiceCaptureFinalVolume);

		/*if (VoiceCaptureFinalVolume > 10) {
			TArray<AActor *> monsters;
			UGameplayStatics::GetAllActorsOfClass(WorldContextObject->GetWorld(), AHypoxiaMonster::StaticClass(), monsters);
			for (AActor * monster : monsters) {
				//Cast<AHypoxiaAIController>(Cast<APawn>(monster)->Controller)->TrackPlayer(VoiceCaptureFinalVolume);
				FVector Loc;
				UHypoxiaSound::PathSoundTo(WorldContextObject, monster->GetActorLocation(), UGameplayStatics::GetPlayerCharacter(WorldContextObject, 0)->GetActorLocation(), Loc);
				UHypoxiaSound::PlaySoundAtLocationRefract(WorldContextObject, testSound, Loc, 1.0f, 1.0f, 0.0f, testAttenuation);
			}
		}*/
	}
}

void UHypoxiaSound::PlaySoundAtLocationRefract(const UObject* WorldContextObject, USoundBase* Sound, FVector Location, float VolumeMultiplier, float PitchMultiplier, float StartTime, USoundAttenuation* Attenuation) {
	
	//UGameplayStatics::PlaySoundAtLocation(WorldContextObject, Sound, Location, VolumeMultiplier, PitchMultiplier, StartTime, Attenuation);
	UGameplayStatics::PlaySoundAtLocation(WorldContextObject, testSound, Location, VolumeMultiplier, PitchMultiplier, StartTime, testAttenuation);
}

void UHypoxiaSound::PathSoundTo(UObject* WorldContextObject, FVector StartLoc, FVector EndLoc, FVector& Loc_Out, float& Vol_Out) {
	FVector ProjectedLocation;
	UNavigationSystem* NavSys = UNavigationSystem::GetCurrent(WorldContextObject);
	FNavPathSharedPtr Path = NavSys->FindPathToLocationSynchronously(WorldContextObject, StartLoc, EndLoc)->GetPath();
	if (!Path.IsValid())
		return;
	TArray<FNavPathPoint> PathPoints = Path->GetPathPoints();
	if (PathPoints.Num() <= 2) {
		Loc_Out = StartLoc;
		Vol_Out = 0.0f;
		return;
	}
	FVector Projection = PathPoints[PathPoints.Num() - 2].Location - PathPoints[PathPoints.Num() - 1].Location;
	//Projection.Normalize();
	// Projects sound location according to distance original sound would have traveled
	ProjectedLocation = EndLoc + Projection /** Path->GetLength()*/;
	//UE_LOG(LogTemp, Warning, TEXT("Sound at = %f, %f, %f"), Projection.X, Projection.Y, Projection.Z);
	Loc_Out = ProjectedLocation;
	Vol_Out = 1.0f - ((Path->GetLength() - Projection.Size()) / 6000.0f);
	UE_LOG(LogTemp, Warning, TEXT("Volume = %f"), Vol_Out);
}


