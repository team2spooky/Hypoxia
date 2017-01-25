// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "Item.h"
#include "MotionControllerComponent.h"
#include "HypoxiaCharacter.h"

AHypoxiaCharacter *HypoxiaCharacter;

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
	//PrimaryActorTick.bCanEverTick = true;

	//FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	//FirstPersonCameraComponent->SetupAttachment(RootComponent);
	//FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 0.f); // Position the camera
	//FirstPersonCameraComponent->bUsePawnControlRotation = false;

	Item_Base = CreateDefaultSubobject<USceneComponent>(TEXT("Item_Base"));

	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->Hand = EControllerHand::Right;
	R_MotionController->SetupAttachment(Item_Base);

	VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	//VR_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	//VR_Gun->bCastDynamicShadow = false;
	//VR_Gun->CastShadow = false;
	VR_Gun->SetupAttachment(R_MotionController);
	VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	for (TObjectIterator<AHypoxiaCharacter> Itr; Itr; ++Itr)
	{

		HypoxiaCharacter = *Itr;
		//break;
		//FString Name = HypoxiaCharacter->GetName();
		//FVector Vec = controller->GetComponentLocation();
		//UE_LOG(LogTemp, Log, TEXT("Logging %s"), *Vec.ToString());
		//if (R_Controller->GetName().Equals(TEXT("R_MotionController"))) {
		//SetActorLocation(R_Controller->GetComponentLocation());
		//break;
		//UE_LOG(LogTemp, Log, TEXT("Hi %s"), *Name);

		

		//}
	}

	if (Item_Base->AttachToComponent(HypoxiaCharacter->GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale)) {
		UE_LOG(LogTemp, Error, TEXT("It worked :)"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("It no worked :("));
	}

}

// Called every frame
void AItem::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	//for (TObjectIterator<AHypoxiaCharacter> Itr; Itr; ++Itr)
	//{

		//AHypoxiaCharacter *Hypo = *Itr;
		//R_Controller = *Itr;
		//FString Name = Hypo->GetName();
		//FVector Vec = controller->GetComponentLocation();
		//UE_LOG(LogTemp, Log, TEXT("Logging %s"), *Vec.ToString());
		//if (R_Controller->GetName().Equals(TEXT("R_MotionController"))) {
		//SetActorLocation(R_Controller->GetComponentLocation());
		//break;
		//UE_LOG(LogTemp, Log, TEXT("Hi %s"), *Name);

	//SetActorLocation(HypoxiaCharacter->GetActorLocation());
		//}
	//}

	//FVector Vec = R_Controller->GetComponentLocation();
	//UE_LOG(LogTemp, Log, TEXT("Location: %s"), *Vec.ToString());
	//SetActorLocation(R_Controller->GetComponentLocation());

}

