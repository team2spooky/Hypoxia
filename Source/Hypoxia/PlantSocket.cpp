// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "PlantSocket.h"


// Sets default values
APlantSocket::APlantSocket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = StaticMesh;

	SnapSpot = CreateDefaultSubobject<USceneComponent>(TEXT("SnapSpot"));
	SnapSpot->SetupAttachment(StaticMesh);
}

// Called when the game starts or when spawned
void APlantSocket::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> matches;
	UGameplayStatics::GetAllActorsWithTag(this->GetWorld(), this->Tags[0], matches);
	for (AActor* match : matches) {
		if (match->IsA(APowerableObject::StaticClass())) {
			TargetObject = Cast<APowerableObject>(match);
			return;
		}
	}
}

// Called every frame
void APlantSocket::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void APlantSocket::Power(float Volume)
{
	if (Volume == 0.f) {
		ObjectPower = 0;
	} else {
		ObjectPower = FMath::Min(1.f, ObjectPower + 0.05f);
	}
	if (IsValid(TargetObject)) {
		TargetObject->Power(ObjectPower);
	}
}
