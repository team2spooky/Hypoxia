// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "HypoxiaAIController.h"
#include "HypoxiaCharacter.h"
#include "EngineUtils.h"
#include "Runtime/AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "Runtime/Engine/Classes/Engine/TriggerBox.h"

EGoal Goal;
FVector GoalPoint;
FVector SoundPoint;
bool HeardSound;
bool EventRunning;
float InvestigateTime;
float WanderTime;
float WanderDistance;

TArray<FVector> TriggerArray;
TArray<TArray<FVector>> TriggerData;

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

	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		
		if (ActorItr->ActorHasTag(TEXT("Trigger"))) {
			
			AActor *Trigger = *ActorItr;
			//TriggerArray.Add(Trigger->GetActorLocation());
			TArray<FVector> Data;
			auto Comps = Trigger->GetComponents().Array();
			//UE_LOG(LogTemp, Warning, TEXT("Comps %i"), Comps.Num());
			for (auto CompItr(Comps.CreateIterator()); CompItr; ++CompItr) {
				auto Comp = Cast<USceneComponent>(*CompItr);
				if (Comp->ComponentHasTag(TEXT("Base"))) {
					TriggerArray.Add(Comp->GetComponentLocation());
					//UE_LOG(LogTemp, Warning, TEXT("CompsLocBase %f"), Comp->GetComponentLocation().X);
					break;
				}
			}
			for (auto CompItr(Comps.CreateIterator()); CompItr; ++CompItr) {
				auto Comp = Cast<USceneComponent>(*CompItr);
				if (Comp->ComponentHasTag(TEXT("Start"))) {
					Data.Add(Comp->GetComponentLocation());
					//UE_LOG(LogTemp, Warning, TEXT("CompsLocStart %f"), Comp->GetComponentLocation().X);
					break;
				}
			}
			for (auto CompItr(Comps.CreateIterator()); CompItr; ++CompItr) {
				auto Comp = Cast<USceneComponent>(*CompItr);
				if (Comp->ComponentHasTag(TEXT("End"))) {
					Data.Add(Comp->GetComponentLocation());
					//UE_LOG(LogTemp, Warning, TEXT("CompsLocEnd %f"), Comp->GetComponentLocation().X);
					break;
				}
			}
			//Data.Add(Comp->GetComponentLocation());
			//Data.Add(Comps[0]->get)
			//Data.Add(Trigger->GetComponents[1].GetComponentLocation());
			TriggerData.Add(Data);
			//UE_LOG(LogTemp, Warning, TEXT("CompsLoc %f"), TriggerData[0][1].X);
			//UE_LOG(LogTemp, Warning, TEXT("CompsLoc %f"), TriggerData[0][1].X);
		}
		
	}

	Goal = EGoal::Wander;

	GoalPoint = GetPawn()->GetActorLocation();
	SoundPoint = FVector(0.0f, 0.0f, 0.0f);
	HeardSound = false;
	EventRunning = false;
	InvestigateTime = 0.0f;
	WanderTime = 0.0f;
	WanderDistance = 1000.0f;

}

void AHypoxiaAIController::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);

	CheckForPlayer();

	switch (Goal) {
	case EGoal::Idle:
		CheckTrigger();
		break;
	case EGoal::Wander:
		if (AtGoal() || WanderTime <= 0.0f) {
			if (!EventRunning) {
				GoalPoint = NextWanderPoint();
			} else {
				EventRunning = false;
				Goal = EGoal::Idle;
				FVector MonsterLocation = GetPawn()->GetActorLocation();
				GetPawn()->SetActorLocation(FVector(MonsterLocation.X, MonsterLocation.Y, 450.0f));
			}
		} else {
			WanderTime -= DeltaTime;
		}
		break;
	case EGoal::Sound:
		if (HeardSound) {
			GoalPoint = SoundPoint;
			HeardSound = false;
		}
		InvestigateTime -= DeltaTime;
		if (InvestigateTime >= 0.0f) {
			SetGoal(EGoal::Wander);
			GoalPoint = NextWanderPoint();
		}
		break;
	case EGoal::Player:
		GoalPoint = HypoxiaCharacter->GetActorLocation();
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Error determining Monster_AI goal"));
		break;
	}

	UBlackboardComponent* AIBlackboard = UAIBlueprintHelperLibrary::GetBlackboard(this);
	AIBlackboard->SetValueAsVector(FName("WanderLocation"), GoalPoint);

}

void AHypoxiaAIController::CheckForPlayer() {
	if (FVector::Dist(HypoxiaCharacter->GetActorLocation(), GetPawn()->GetActorLocation()) < 200.0f) {
		SetGoal(EGoal::Player);
		FVector MonsterLocation = GetPawn()->GetActorLocation();
		GetPawn()->SetActorLocation(FVector(MonsterLocation.X, MonsterLocation.Y, 75.0f));
	}
}

void AHypoxiaAIController::CheckTrigger() {
	for (auto TriggerItr(TriggerArray.CreateIterator()); TriggerItr; ++TriggerItr) {
		auto Trigger = *TriggerItr;
		if (FVector::Dist(Trigger, HypoxiaCharacter->GetActorLocation()) < 300.0f) {
			FVector Start = TriggerData[TriggerItr.GetIndex()][0];
			FVector End   = TriggerData[TriggerItr.GetIndex()][1];
			GetPawn()->SetActorLocation(Start);
			GoalPoint = End;
			WanderTime = 600.0f;
			EventRunning = true;
			SetGoal(EGoal::Wander);
			TriggerArray.RemoveAt(TriggerItr.GetIndex(), 1, true);
			TriggerData.RemoveAt(TriggerItr.GetIndex(), 1, true);
			break;
		}
	}
}

void AHypoxiaAIController::SetGoal(EGoal NewGoal) {
	if (Goal == EGoal::Player) {
		return;
	} else if(NewGoal != EGoal::Idle) {
		Goal = NewGoal;
	}
}

bool AHypoxiaAIController::AtGoal() {
	return FVector::Dist(GoalPoint, GetPawn()->GetActorLocation()) < 200.0f;
}

FVector AHypoxiaAIController::NextWanderPoint() {
	FVector MonsterLocation = GetPawn()->GetActorLocation();
	FVector PlayerLocation  = HypoxiaCharacter->GetActorLocation();
	
	GetPawn()->SetActorLocation(FVector(MonsterLocation.X, MonsterLocation.Y, 450.0f));

	float PointAngle = FMath::RandRange(-180, 180);
	float XDist = FMath::Sin(PointAngle) * WanderDistance;
	float YDist = FMath::Cos(PointAngle) * WanderDistance;

	FVector NewWanderPoint = FVector(PlayerLocation.X + XDist, PlayerLocation.Y + YDist, 450.0f);

	//UE_LOG(LogTemp, Warning, TEXT("XPoint %f"), PlayerLocation.X + XDist);
	//UE_LOG(LogTemp, Warning, TEXT("YPoint %f"), PlayerLocation.Y + YDist);

	WanderTime = 10.0f;

	return NewWanderPoint;
}

void AHypoxiaAIController::HearSound(FVector Location, float Amplitude) {

	HeardSound = true;

	SetGoal(EGoal::Sound);

	SoundPoint = Location;

	InvestigateTime = 18.0f;

	FVector MonsterLocation = GetPawn()->GetActorLocation();

	GetPawn()->SetActorLocation(FVector(MonsterLocation.X, MonsterLocation.Y, 75.0f));

}
