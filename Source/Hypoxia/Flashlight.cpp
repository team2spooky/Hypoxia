// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "Flashlight.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "MotionControllerComponent.h"
#include "EngineUtils.h"
#include "Engine/StaticMeshActor.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

AFlashlight::AFlashlight()
{

	//Light_MotionController = GetDefaultSubobjectByName(TEXT("MotionController"));

	Light = CreateDefaultSubobject<USpotLightComponent>(TEXT("Light"));
	Light->SetupAttachment(Item);

	//Text = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	//Text->SetupAttachment(Item_Base);
}

void AFlashlight::BeginPlay() {

	Super::BeginPlay();

	Alive = true;

	DynamicMaterial = Item->CreateAndSetMaterialInstanceDynamic(0);
	//for (TActorIterator<ADoor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	//{
	//	if (ActorItr->ActorHasTag(Tags[0])) {

	//		Door = *ActorItr;
	//		UE_LOG(LogTemp, Warning, TEXT("Child: %s"), *ActorItr->GetName());
	//		UE_LOG(LogTemp, Warning, TEXT("Parent: %s"), *GetName());
	//		break;
	//	}
	//}
}

void AFlashlight::Use() {
	
	if (Alive) {

		float intensity = Light->Intensity;
		if (intensity == 0.0f) {
			Light->SetIntensity(6000.0f);
			// Play a sound when the flashlight is clicked on
			if (ClickOnSound != NULL) {
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), ClickOnSound, GetActorLocation());
			}
		} else {
			Light->SetIntensity(0.0f);
			// Play a sound when the flashlight is clicked off
			if (ClickOffSound != NULL) {
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), ClickOffSound, GetActorLocation());
			}
		}
	}

	//Text->SetText("This is where the text for something long<br>that the player would read<br>could go.<br><br>As you can see the light on it is strange but could be cool<br>right now you have to shine<br>the light on it<br>to really be able to read it.");
	//Text->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));

	//Rotation_Time = 240;

	//Drop();
	//Item->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f), false, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);
}

bool AFlashlight::Pickup(USceneComponent* Controller, EControllerHand Hand) {
	//UE_LOG(LogTemp, Warning, TEXT("Dist %f"), FVector::Dist(Controller->GetComponentLocation(), Item_Base->GetComponentLocation()));

	bool RetVal = Super::Pickup(Controller, Hand);

	OnPickUp.Broadcast();

	//Door->Unlock();

	return RetVal;

}

void AFlashlight::KillLight() {
	Light->SetIntensity(0.0f);
	Alive = false;
	DynamicMaterial->SetScalarParameterValue(FName("FlashlightGlow"), 0.f);
}

