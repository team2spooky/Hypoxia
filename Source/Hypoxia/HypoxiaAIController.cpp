// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "HypoxiaAIController.h"
#include "HypoxiaCharacter.h"
#include "HypoxiaMonster.h"
#include "EngineUtils.h"
#include "Runtime/AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"

//void AHypoxiaAIController::TrackPlayer(float volume) {
//	FVector playerLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
//	FVector* vector = new FVector(FMath::Lerp(400, 50, volume/100), 0.0f, 0.0f);
//	vector->RotateAngleAxis(FMath::Lerp(0, 359, FMath::Rand() / RAND_MAX), vector->UpVector);
//	MoveToLocation(playerLocation + *vector, 5.0f);
//
//	UE_LOG(LogTemp, Warning, TEXT("Chase!"));
//}

float MoveTimer = 10.f;
float Looktime  = 1.0f;

AHypoxiaAIController::AHypoxiaAIController() {
	//Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	//Blackboard->SetupAttachment(Item_Base);
}

void AHypoxiaAIController::BeginPlay() {
	Super::BeginPlay();

	for (TActorIterator<AHypoxiaCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		//This may need adjusting to ensure it gets the right one
		HypoxiaCharacter = *ActorItr;
	}

	//UE_LOG(LogTemp, Warning, TEXT("LoS %d"), LineOfSightTo(HypoxiaCharacter));

	//UBlackboardComponent* AIBlackboard = UAIBlueprintHelperLibrary::GetBlackboard(this);

	//AIBlackboard->SetValueAsVector(FName("WanderLocation"), FVector(1050.0f, 500.0f, 90.0f));
	//AIBlackboard->SetValueAsVector(FName("WanderLocation"), FVector(-700.0f, -1040.0f, 118.0f));

}

void AHypoxiaAIController::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("Dist %F"), FVector::Dist(HypoxiaCharacter->GetActorLocation(), GetPawn()->GetActorLocation()));

	//UE_LOG(LogTemp, Warning, TEXT("LoS %d"), LineOfSightTo(HypoxiaCharacter) && FVector::Dist(HypoxiaCharacter->GetActorLocation(), GetPawn()->GetActorLocation()) < 200.0f);

	UBlackboardComponent* AIBlackboard = UAIBlueprintHelperLibrary::GetBlackboard(this);

	if (AIBlackboard->GetValueAsBool(FName("SeenPlayer"))) {
		AIBlackboard->SetValueAsVector(FName("PlayerLocation"), HypoxiaCharacter->GetActorLocation());
	} else if (LineOfSightTo(HypoxiaCharacter) && FVector::Dist(HypoxiaCharacter->GetActorLocation(), GetPawn()->GetActorLocation()) < 200.0f) {
		AIBlackboard->SetValueAsBool(FName("SeenPlayer"), true);
		AHypoxiaMonster* Monster = Cast<AHypoxiaMonster>(this->GetPawn());
		UComplexAudioComponent* DetectSoundComponent = NewObject<UComplexAudioComponent>(Monster, FName("DynamicSound"));
		DetectSoundComponent->bAutoDestroy = true;
		DetectSoundComponent->bAdvancedOcclusion = true;
		DetectSoundComponent->ProjectedVolume = 100;
		DetectSoundComponent->Radius = 50;
		DetectSoundComponent->SetupAttachment(Monster->GetRootComponent());
		DetectSoundComponent->RegisterComponent();
		DetectSoundComponent->SetAttenuationSettings(Monster->DetectAttenuation);
		DetectSoundComponent->SetSound(Monster->DetectSound);
		DetectSoundComponent->Play();
		//AIBlackboard->SetValueAsVector(FName("PlayerLocation"), HypoxiaCharacter->GetActorLocation());
	}

	MoveTimer -= DeltaTime;
	Looktime  -= DeltaTime;

	if (MoveTimer <= 0.0f) {
		MoveTimer = 30.f;
		AIBlackboard->SetValueAsVector(FName("WanderLocation"), FVector(FMath::RandRange(1810.f, 4880.0f), FMath::RandRange(-1770.f, 430.0f), 90.0f));
	}

	if (Looktime <= 0.0f) {
		AIBlackboard->SetValueAsBool(FName("HeardSound"), false);
	}
}

void AHypoxiaAIController::HearSound(FVector Location, float Amplitude) {
	
	UBlackboardComponent* AIBlackboard = UAIBlueprintHelperLibrary::GetBlackboard(this);
	AIBlackboard->SetValueAsBool(FName("HeardSound"), true);
	AIBlackboard->SetValueAsVector(FName("SoundLocation"), Location);

	Looktime = 5.0f;

	//UE_LOG(LogTemp, Warning, TEXT("SOUNDDDDDD!!!!!"));
}