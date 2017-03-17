// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "HypoxiaAIController.h"
#include "HypoxiaCharacter.h"
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

int RandNum = 0;
float MoveTimer = 1.0f;
float Looktime = 1.0f;
float WanderTimer = 3.0f;

//leaving the airlock
float TriggerPtX1 = 2270.0f;
float TriggerPtX2 = 2390.0f;
float TriggerPtY1 = -930.0f;
float TriggerPtY2 = -950.0f;

//chair room
float MPoint1X = 2740.0f;
float MPoint1Y = 300.0f;
float MPoint1Z = 20.0f;
//end of hall room and script location
float MPoint2X = 4820.0f;
float MPoint2Y = 370.0f;
float MPoint2Z = 20.0f;
//office room
float MPoint3X = 3960.0f;
float MPoint3Y = -160.0f;
float MPoint3Z = 20.0f;
//main lab
float MPoint4X = 3100.0f;
float MPoint4Y = -1380.0f;
float MPoint4Z = 20.0f;
//beginning of hall
float MPoint5X = 4550.0f;
float MPoint5Y = -1060.0f;
float MPoint5Z = 20.0f;

//end of hall
float SPointX = 4600.0f; //3740.0f; 
float SPointY = 550.0f; //-580.0f; 
float SPointZ = 20.0f;

// entering the hall
float STriggerX1 = 4380.0f; //4008.0f;
float STriggerY1 = -1200.0f; //1000.0f
float STriggerX2 = 4670.0f; //3900.0f;
float STriggerY2 = -300.0f; //1100.0f


float TempX = 0.0f;
float TempY = 0.0f;

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

	APawn* Pawn = GetPawn();

	//UE_LOG(LogTemp, Warning, TEXT("Player X %f"), HypoxiaCharacter->GetActorLocation().X);
	//UE_LOG(LogTemp, Warning, TEXT("Player Y %f"), HypoxiaCharacter->GetActorLocation().Y);
	//UE_LOG(LogTemp, Warning, TEXT("Monster Z %f"), pawn->GetActorLocation().Z);

	if (AIBlackboard->GetValueAsBool(FName("SeenPlayer"))) {
		AIBlackboard->SetValueAsVector(FName("PlayerLocation"), HypoxiaCharacter->GetActorLocation());
	} else if (LineOfSightTo(HypoxiaCharacter) && FVector::Dist(HypoxiaCharacter->GetActorLocation(), GetPawn()->GetActorLocation()) < 200.0f) {
		AIBlackboard->SetValueAsBool(FName("SeenPlayer"), true);
		//AIBlackboard->SetValueAsVector(FName("PlayerLocation"), HypoxiaCharacter->GetActorLocation());
	}

	if ((HypoxiaCharacter->GetActorLocation().X >= STriggerX1)
		&& (HypoxiaCharacter->GetActorLocation().Y >= STriggerY1)
		&& (HypoxiaCharacter->GetActorLocation().X <= STriggerX2)
		&& (HypoxiaCharacter->GetActorLocation().Y <= STriggerY2)) {
		//UE_LOG(LogTemp, Warning, TEXT("Player at script location"));
		AIBlackboard->SetValueAsBool(FName("AtScriptLocation"), true);

	}



	//AIBlackboard->SetValueAsBool(FName("StartMonster"), true);

	if (AIBlackboard->GetValueAsBool(FName("StartMonster"))) {

		MoveTimer -= DeltaTime;
		Looktime -= DeltaTime;
		WanderTimer -= DeltaTime;
		RandNum = FMath::RandRange(1, 5);
		if (AIBlackboard->GetValueAsBool(FName("AtScriptLocation"))) {
			RandNum = 6;

		}
		//RandNum = 6;

		if (MoveTimer <= 0.0f) {
			MoveTimer = 20.0f;
			UE_LOG(LogTemp, Warning, TEXT("Rand %d"), RandNum);
			switch (RandNum) {
			case 1:
				AIBlackboard->SetValueAsVector(FName("WanderLocation"), FVector(MPoint1X, MPoint1Y, MPoint1Z));
				break;
			case 2:
				AIBlackboard->SetValueAsVector(FName("WanderLocation"), FVector(MPoint2X, MPoint2Y, MPoint2Z));
				break;
			case 3:
				AIBlackboard->SetValueAsVector(FName("WanderLocation"), FVector(MPoint3X, MPoint3Y, MPoint3Z));
				break;
			case 4:
				AIBlackboard->SetValueAsVector(FName("WanderLocation"), FVector(MPoint4X, MPoint4Y, MPoint4Z));
				break;
			case 5:
				AIBlackboard->SetValueAsVector(FName("WanderLocation"), FVector(MPoint5X, MPoint5Y, MPoint5Z));
				break;
			case 6:
				AIBlackboard->SetValueAsVector(FName("WanderLocation"), FVector(SPointX, SPointY, SPointZ));
				AIBlackboard->SetValueAsBool(FName("AtScriptLocation"), false);
				break;
			default:
				AIBlackboard->SetValueAsVector(FName("WanderLocation"), FVector(FMath::RandRange(1810.f, 4880.0f), FMath::RandRange(-1770.f, 430.0f), 20.0f));
				UE_LOG(LogTemp, Warning, TEXT("Path"));
				break;
			}

			if (Looktime <= 0.0f) {
				AIBlackboard->SetValueAsBool(FName("HeardSound"), false);
			}
			

		}

		if (AIBlackboard->GetValueAsBool(FName("AtWanderLocation"))) {
			UE_LOG(LogTemp, Warning, TEXT("Monster's moving around the Location"));
			TempX = AIBlackboard->GetValueAsVector(FName("WanderLocation")).X;
			TempY = AIBlackboard->GetValueAsVector(FName("WanderLocation")).Y;
			TempX += FMath::RandRange(-75.0f, 75.0f);
			TempY += FMath::RandRange(-75.0f, 75.0f);
			AIBlackboard->SetValueAsVector(FName("WanderLocation"), FVector(TempX, TempY, 90));
			AIBlackboard->SetValueAsBool(FName("AtWanderLocation"), false);
			WanderTimer = 3.0f;
		} else if ((Pawn->GetActorLocation().X >= AIBlackboard->GetValueAsVector(FName("WanderLocation")).X - 50.0f)
			&& (Pawn->GetActorLocation().X <= AIBlackboard->GetValueAsVector(FName("WanderLocation")).X + 50.0f)
			&& (WanderTimer <= 0.0f)
			//&& (pawn->GetActorLocation().Z <= AIBlackboard->GetValueAsVector(FName("WanderLocation")).Z + 1.0f)
			//&& (pawn->GetActorLocation().Z >= AIBlackboard->GetValueAsVector(FName("WanderLocation")).Z - 71.0f)) {
			) {
			AIBlackboard->SetValueAsBool(FName("AtWanderLocation"), true);
			UE_LOG(LogTemp, Warning, TEXT("Monster's at the Location"));
		}
	} else if ((HypoxiaCharacter->GetActorLocation().X >= TriggerPtX1)
		&& (HypoxiaCharacter->GetActorLocation().X <= TriggerPtX2)
		&& (HypoxiaCharacter->GetActorLocation().Y <= TriggerPtY1)
		&& (HypoxiaCharacter->GetActorLocation().Y >= TriggerPtY2)) {
		//UE_LOG(LogTemp, Warning, TEXT("Monster's Moving Around"));
		AIBlackboard->SetValueAsBool(FName("StartMonster"), true);
		AIBlackboard->SetValueAsBool(FName("AtScriptLocation"), true);
	}

}

void AHypoxiaAIController::HearSound(FVector Location, float Amplitude) {

	UBlackboardComponent* AIBlackboard = UAIBlueprintHelperLibrary::GetBlackboard(this);
	AIBlackboard->SetValueAsBool(FName("HeardSound"), true);
	AIBlackboard->SetValueAsVector(FName("SoundLocation"), Location);

	Looktime = 1.0f;

	//UE_LOG(LogTemp, Warning, TEXT("SOUNDDDDDD!!!!!"));
}
