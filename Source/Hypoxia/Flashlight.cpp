// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "Flashlight.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "MotionControllerComponent.h"

AFlashlight::AFlashlight()
{

	//Light_MotionController = GetDefaultSubobjectByName(TEXT("MotionController"));

	Light = CreateDefaultSubobject<USpotLightComponent>(TEXT("Light"));
	Light->SetupAttachment(Item);

	//Text = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	//Text->SetupAttachment(Item_Base);
}

void AFlashlight::Use() {
	
	float intensity = Light->Intensity;
	if (intensity == 0.0f) {
		Light->SetIntensity(6000.0f);
	} else {
		Light->SetIntensity(0.0f);
	}

	//Text->SetText("This is where the text for something long<br>that the player would read<br>could go.<br><br>As you can see the light on it is strange but could be cool<br>right now you have to shine<br>the light on it<br>to really be able to read it.");
	//Text->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));

	//Rotation_Time = 240;

	//Drop();
	//Item->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f), false, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);
}

//void AFlashlight::Tick(float DeltaTime) {
//	
//	Super::Tick(DeltaTime);
//
//	/*if (Rotation_Time >= 0) {
//		Rotation_Time--;
//		Item->SetRelativeRotation(FRotator(Item->GetComponentRotation().Pitch + 1.0f, 0.0f, 0.0f), false, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);
//	}*/
//}
