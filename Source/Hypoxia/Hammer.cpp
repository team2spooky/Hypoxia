// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "Hammer.h"
#include "EngineUtils.h"
#include "PhysicsEngine/DestructibleActor.h"
#include "Components/DestructibleComponent.h"

void AHammer::BeginPlay() {
	Super::BeginPlay();

	//for (TActorIterator<ADestructibleActor> ActorItr(GetWorld()); ActorItr; ++ActorItr) {

	//	ADestructibleActor *Mesh = *ActorItr;

	//	if (ActorItr->ActorHasTag(Tags[0])) {

	//		//Door = *ActorItr;

	//		UE_LOG(LogTemp, Warning, TEXT("Child: %s"), *ActorItr->GetName());
	//		UE_LOG(LogTemp, Warning, TEXT("Parent: %s"), *GetName());
	//		break;
	//	}
	//}
}


void AHammer::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) {
	//UE_LOG(LogTemp, Warning, TEXT("I'm hit, I'm hit"));

	if (OtherActor->IsA(ADestructibleActor::StaticClass())) {
		UE_LOG(LogTemp, Warning, TEXT("I'm hit, I'm hit"));
		ADestructibleActor* Destruct = Cast<ADestructibleActor>(OtherActor);
		//UDestructibleComponent* CompDesc = Cast<UDestructibleComponent>(OtherComponent);
		
		//Destruct->GetDestructibleComponent()->SetSimulatePhysics(true);
		Destruct->GetDestructibleComponent()->ApplyDamage(1.0f, Hit.Location, NormalImpulse, 10000.0f);
		//Destruct->GetDestructibleComponent()->AddForce(FVector(0.0f, 0.0f, 5.0f));
	}
}