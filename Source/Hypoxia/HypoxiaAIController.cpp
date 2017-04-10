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
			UE_LOG(LogTemp, Warning, TEXT("Comps %i"), Comps.Num());
			for (auto CompItr(Comps.CreateIterator()); CompItr; ++CompItr) {
				auto Comp = Cast<USceneComponent>(*CompItr);
				if (Comp->ComponentHasTag(TEXT("Base"))) {
					TriggerArray.Add(Comp->GetComponentLocation());
					UE_LOG(LogTemp, Warning, TEXT("CompsLocBase %f"), Comp->GetComponentLocation().X);
					break;
				}
			}
			for (auto CompItr(Comps.CreateIterator()); CompItr; ++CompItr) {
				auto Comp = Cast<USceneComponent>(*CompItr);
				if (Comp->ComponentHasTag(TEXT("Start"))) {
					Data.Add(Comp->GetComponentLocation());
					UE_LOG(LogTemp, Warning, TEXT("CompsLocStart %f"), Comp->GetComponentLocation().X);
					break;
				}
			}
			for (auto CompItr(Comps.CreateIterator()); CompItr; ++CompItr) {
				auto Comp = Cast<USceneComponent>(*CompItr);
				if (Comp->ComponentHasTag(TEXT("End"))) {
					Data.Add(Comp->GetComponentLocation());
					UE_LOG(LogTemp, Warning, TEXT("CompsLocEnd %f"), Comp->GetComponentLocation().X);
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
		
		/*ATriggerBox *Trigger = *ActorItr;
		TriggerArray.Add(Trigger->GetActorLocation());
		TArray<float> Data;
		Data.Init(Trigger->Tags);
		TriggerData.Add(Data);*/
	}

	Goal = EGoal::Wander;

	GoalPoint = GetPawn()->GetActorLocation();
	SoundPoint = FVector(0.0f, 0.0f, 0.0f);
	HeardSound = false;
	InvestigateTime = 0.0f;
	WanderTime = 0.0f;
	WanderDistance = 1000.0f;

	//UE_LOG(LogTemp, Warning, TEXT("LoS %d"), LineOfSightTo(HypoxiaCharacter));

	//UBlackboardComponent* AIBlackboard = UAIBlueprintHelperLibrary::GetBlackboard(this);

	//AIBlackboard->SetValueAsVector(FName("WanderLocation"), FVector(1050.0f, 500.0f, 90.0f));
	//AIBlackboard->SetValueAsVector(FName("WanderLocation"), FVector(-700.0f, -1040.0f, 118.0f));

}

void AHypoxiaAIController::TriggerEvent() {

}

//void AHypoxiaAIController::RegisterDelegate()
//{
//	
//}
//
//void AMyActorWhichRegisteresWithABox::OnBeginTriggerOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	// This gets called when an actor begins to overlap with MyBoxComponent
//}

void AHypoxiaAIController::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);

	CheckForPlayer();

	switch (Goal) {
	case EGoal::Idle:
		CheckTrigger();
		break;
	case EGoal::Wander:
		if (AtGoal() || WanderTime <= 0.0f) {
			GoalPoint = NextWanderPoint();
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

	////UE_LOG(LogTemp, Warning, TEXT("Dist %F"), FVector::Dist(HypoxiaCharacter->GetActorLocation(), GetPawn()->GetActorLocation()));
	////UE_LOG(LogTemp, Warning, TEXT("LoS %d"), LineOfSightTo(HypoxiaCharacter) && FVector::Dist(HypoxiaCharacter->GetActorLocation(), GetPawn()->GetActorLocation()) < 200.0f);
	//UBlackboardComponent* AIBlackboard = UAIBlueprintHelperLibrary::GetBlackboard(this);
	//APawn* AIPawn = GetPawn();
	////UE_LOG(LogTemp, Warning, TEXT("Player X %f"), HypoxiaCharacter->GetActorLocation().X);
	////UE_LOG(LogTemp, Warning, TEXT("Player Y %f"), HypoxiaCharacter->GetActorLocation().Y);
	////UE_LOG(LogTemp, Warning, TEXT("Monster Z %f"), pawn->GetActorLocation().Z);
	//if (LineOfSightTo(HypoxiaCharacter) && FVector::Dist(HypoxiaCharacter->GetActorLocation(), GetPawn()->GetActorLocation()) < 200.0f) {
	//	AIBlackboard->SetValueAsBool(FName("SeenPlayer"), true);
	//}
	//if (AIBlackboard->GetValueAsBool(FName("SeenPlayer"))) {
	//	AIBlackboard->SetValueAsVector(FName("PlayerLocation"), HypoxiaCharacter->GetActorLocation());
	//}
	//AIBlackboard->SetValueAsBool(FName("StartMonster"), true);
	//if (AIBlackboard->GetValueAsBool(FName("StartMonster"))) {
	//	MoveTimer -= DeltaTime;
	//	Looktime -= DeltaTime;
	//	WanderTimer -= DeltaTime;
	//	if (Looktime <= 0.0f) {
	//		AIBlackboard->SetValueAsBool(FName("HeardSound"), false);
	//	}
	//	//RandNum = FMath::RandRange(1, 5);
	//	//RandNum = 6;
	//	/*if (MoveTimer <= 0.0f) {
	//		MoveTimer = 20.0f;
	//		UE_LOG(LogTemp, Warning, TEXT("Rand %d"), RandNum);
	//		switch (RandNum) {
	//		case 1:
	//			AIBlackboard->SetValueAsVector(FName("WanderLocation"), FVector(MPoint1X, MPoint1Y, MPoint1Z));
	//			break;
	//		case 2:
	//			AIBlackboard->SetValueAsVector(FName("WanderLocation"), FVector(MPoint2X, MPoint2Y, MPoint2Z));
	//			break;
	//		case 3:
	//			AIBlackboard->SetValueAsVector(FName("WanderLocation"), FVector(MPoint3X, MPoint3Y, MPoint3Z));
	//			break;
	//		case 4:
	//			AIBlackboard->SetValueAsVector(FName("WanderLocation"), FVector(MPoint4X, MPoint4Y, MPoint4Z));
	//			break;
	//		case 5:
	//			AIBlackboard->SetValueAsVector(FName("WanderLocation"), FVector(MPoint5X, MPoint5Y, MPoint5Z));
	//			break;
	//		default:
	//			AIBlackboard->SetValueAsVector(FName("WanderLocation"), FVector(FMath::RandRange(1810.f, 4880.0f), FMath::RandRange(-1770.f, 430.0f), 20.0f));
	//			UE_LOG(LogTemp, Warning, TEXT("Path"));
	//			break;
	//		}
	//	}*/
	//	UE_LOG(LogTemp, Warning, TEXT("Dist %f"), FVector::Dist(AIBlackboard->GetValueAsVector(FName("WanderLocation")), GetPawn()->GetActorLocation()));
	//	if (MoveTimer <= 0.0f) {
	//		AIBlackboard->SetValueAsVector(FName("WanderLocation"), FVector(FMath::RandRange(1810.f, 4880.0f), FMath::RandRange(-1770.f, 430.0f), 70.0f));
	//		MoveTimer = 20.0f;
	//	} else if (FVector::Dist(AIBlackboard->GetValueAsVector(FName("WanderLocation")), GetPawn()->GetActorLocation()) < 200.0f){
	//		FVector NewWander = FVector(AIBlackboard->GetValueAsVector(FName("WanderLocation")).X + FMath::RandRange(-10.0f, 10.0f), AIBlackboard->GetValueAsVector(FName("WanderLocation")).Y + FMath::RandRange(-10.0f, 10.0f), AIBlackboard->GetValueAsVector(FName("WanderLocation")).Z + FMath::RandRange(-10.0f, 10.0f));
	//		AIBlackboard->SetValueAsVector(FName("WanderLocation"), FVector());
	//	}
	//}
	////	if (AIBlackboard->GetValueAsBool(FName("AtWanderLocation"))) {
	////		UE_LOG(LogTemp, Warning, TEXT("Monster's moving around the Location"));
	////		TempX = AIBlackboard->GetValueAsVector(FName("WanderLocation")).X;
	////		TempY = AIBlackboard->GetValueAsVector(FName("WanderLocation")).Y;
	////		TempX += FMath::RandRange(-75.0f, 75.0f);
	////		TempY += FMath::RandRange(-75.0f, 75.0f);
	////		AIBlackboard->SetValueAsVector(FName("WanderLocation"), FVector(TempX, TempY, 90));
	////		AIBlackboard->SetValueAsBool(FName("AtWanderLocation"), false);
	////		WanderTimer = 3.0f;
	////	} else if ((Pawn->GetActorLocation().X >= AIBlackboard->GetValueAsVector(FName("WanderLocation")).X - 50.0f)
	////		&& (Pawn->GetActorLocation().X <= AIBlackboard->GetValueAsVector(FName("WanderLocation")).X + 50.0f)
	////		&& (WanderTimer <= 0.0f)
	////		//&& (pawn->GetActorLocation().Z <= AIBlackboard->GetValueAsVector(FName("WanderLocation")).Z + 1.0f)
	////		//&& (pawn->GetActorLocation().Z >= AIBlackboard->GetValueAsVector(FName("WanderLocation")).Z - 71.0f)) {
	////		) {
	////		AIBlackboard->SetValueAsBool(FName("AtWanderLocation"), true);
	////		UE_LOG(LogTemp, Warning, TEXT("Monster's at the Location"));
	////	}
	////} else if ((HypoxiaCharacter->GetActorLocation().X >= TriggerPtX1)
	////	&& (HypoxiaCharacter->GetActorLocation().X <= TriggerPtX2)
	////	&& (HypoxiaCharacter->GetActorLocation().Y <= TriggerPtY1)
	////	&& (HypoxiaCharacter->GetActorLocation().Y >= TriggerPtY2)) {
	////	UE_LOG(LogTemp, Warning, TEXT("Monster's Moving Around"));
	////	AIBlackboard->SetValueAsBool(FName("StartMonster"), true);

}

void AHypoxiaAIController::CheckForPlayer() {
	if (FVector::Dist(HypoxiaCharacter->GetActorLocation(), GetPawn()->GetActorLocation()) < 200.0f) {
		SetGoal(EGoal::Player);
		FVector MonsterLocation = GetPawn()->GetActorLocation();
		GetPawn()->SetActorLocation(FVector(MonsterLocation.X, MonsterLocation.Y, 75.0f));
	}
}

void AHypoxiaAIController::CheckTrigger() {

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

	UE_LOG(LogTemp, Warning, TEXT("XPoint %f"), PlayerLocation.X + XDist);
	UE_LOG(LogTemp, Warning, TEXT("YPoint %f"), PlayerLocation.Y + YDist);

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
