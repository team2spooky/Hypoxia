// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "Item.h"
#include "MotionControllerComponent.h"
#include "HypoxiaCharacter.h"
#include "EngineUtils.h"
#include "Door.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"

AHypoxiaCharacter *HypoxiaCharacter;

const float ARM_STRENGTH = 400.0f; //Applifies the velocity of objects when dropped

USoundAttenuation* DefaultAttenuation;

// Sets default values
AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	Item_Base = CreateDefaultSubobject<USceneComponent>(TEXT("Item_Base"));
	RootComponent = Item_Base;

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	MotionController->Hand = EControllerHand::Right;
	MotionController->SetupAttachment(Item_Base);
	MotionController->bAbsoluteLocation = true;
	MotionController->bAbsoluteRotation = true;

	MotionTracker = CreateDefaultSubobject<USceneComponent>(TEXT("MotionTracker"));
	MotionTracker->SetupAttachment(Item_Base);

	Item = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item"));
	Item->SetupAttachment(Item_Base);
	Item->SetSimulatePhysics(true);

	Item->GetBodyInstance()->bLockTranslation = false;
	Item->GetBodyInstance()->bLockRotation = false;

	Item->SetNotifyRigidBodyCollision(true);
	Item->OnComponentHit.AddDynamic(this, &AItem::OnHit);
	Item->SetCanEverAffectNavigation(false);

	GrabSpot = CreateDefaultSubobject<USceneComponent>(TEXT("GrabSpot"));
	GrabSpot->SetupAttachment(Item);

	/*HitSoundComponent = CreateDefaultSubobject<UComplexAudioComponent>(TEXT("HitSoundComponent"));
	HitSoundComponent->SetupAttachment(Item_Base);
	HitSoundComponent->SetSound(OnHitSound);*/
	static ConstructorHelpers::FObjectFinder<USoundAttenuation> AttenuationAsset((TEXT("/Game/DefaultAttenuation.DefaultAttenuation")));
	if (AttenuationAsset.Succeeded())
		DefaultAttenuation = AttenuationAsset.Object;
}

// Called when the game starts or when spawned
void AItem::BeginPlay() {

	Super::BeginPlay();

	Held = false;

	OldLocation = FVector();
	NewLocation = FVector();

	bHasGravity = Item->IsGravityEnabled();

	//Item->SetEnableGravity(true);

	for (TActorIterator<AHypoxiaCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		//This may need adjusting to ensure it gets the right one
		HypoxiaCharacter = *ActorItr;
	}

}

bool AItem::Pickup(USceneComponent* Controller, EControllerHand Hand) {
	//UE_LOG(LogTemp, Warning, TEXT("Dist %f"), FVector::Dist(Controller->GetComponentLocation(), Item_Base->GetComponentLocation()));

	if (!Held) {

		//If the item is within the given distance of the given controller, pick it up
		if (FVector::Dist(Controller->GetComponentLocation(), GrabSpot->GetComponentLocation()) < 10.0f) {

			//Item->SetSimulatePhysics(false);
			Item->SetEnableGravity(false);
			Item->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, 0.0f));
			Item->SetPhysicsAngularVelocity(FVector(0.0f, 0.0f, 0.0f));

			//Attach the components, don't move the item if something fails with that
			if (Item_Base->AttachToComponent(HypoxiaCharacter->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale)) {
				HypoxiaCharacter->SetHeldItem(this, Hand);
				Item->AttachToComponent(MotionTracker, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				MotionController->Hand = Hand;
				Held = true;
				return true;
			}
		}
	}

	return false;

}

void AItem::Drop() {

	if (Held) {
		Held = false;
		UE_LOG(LogTemp, Warning, TEXT("Dropped"));
		Item->SetEnableGravity(bHasGravity);
		//Item->SetPhysicsLinearVelocity(MotionController->GetPhysicsLinearVelocity());
		Item_Base->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		Item->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		Item->AddImpulse(FVector(((NewLocation - OldLocation) * ARM_STRENGTH).X, ((NewLocation - OldLocation) * ARM_STRENGTH).Y, ((NewLocation - OldLocation) * ARM_STRENGTH).Z));
	}
}

void AItem::SelfDrop() {
	if (Held) {
		if (MotionController->Hand == EControllerHand::Right) {
			HypoxiaCharacter->ItemPickupRight();
		} else if (MotionController->Hand == EControllerHand::Left) {
			HypoxiaCharacter->ItemPickupLeft();
		}
	}
}

void AItem::Use() {}

void AItem::UpdatePosition(FVector DevicePosition) {

	MotionTracker->SetWorldLocation(MotionController->GetComponentLocation() + RootComponent->GetComponentLocation() - DevicePosition + FVector(0.0f, 0.0f, DevicePosition.Z - 100.f), false, (FHitResult*)nullptr, ETeleportType::None);

	OldLocation = NewLocation;
	NewLocation = MotionTracker->GetComponentLocation();

	if (!Item->GetBodyInstance()->bLockTranslation) {
		Item->SetWorldLocation(MotionTracker->GetComponentLocation(), true, (FHitResult*)nullptr, ETeleportType::None);
	}

}

void AItem::UpdateRotation(FRotator DeviceRotation) {
	
	MotionTracker->SetWorldRotation(MotionController->GetComponentRotation() + RootComponent->GetComponentRotation() - FRotator(0.0f, DeviceRotation.Yaw, 0.0f));
	
	//FRotator NewRotator = FRotator(0.0f, 0.0f, 0.0f);

	//If someone's bored, make these use the ? operator
	//if (!Item->GetBodyInstance()->bLockXRotation) {
	//	NewRotator.Pitch = MotionTracker->GetComponentRotation().Pitch;
	//}
	//else {
	//	NewRotator.Pitch = Item->GetComponentRotation().Pitch;
	//}

	//if (!Item->GetBodyInstance()->bLockYRotation) {
	//	NewRotator.Yaw = MotionTracker->GetComponentRotation().Yaw;
	//}
	//else {
	//	NewRotator.Yaw = Item->GetComponentRotation().Yaw;
	//}

	//if (!Item->GetBodyInstance()->bLockZRotation) {
	//	NewRotator.Roll = MotionTracker->GetComponentRotation().Pitch * -1.0f;// +FGenericPlatformMath::Abs(((NewLocation - OldLocation) * 20.0f).Y);
	//}
	//else {
	//	NewRotator.Roll = Item->GetComponentRotation().Pitch;
	//}

	//float YLength = FGenericPlatformMath::Abs(MotionTracker->GetComponentLocation().Y - Item->GetComponentLocation().Y);
	//float ZLength = FGenericPlatformMath::Abs(MotionTracker->GetComponentLocation().Z - Item->GetComponentLocation().Z);

	//UE_LOG(LogTemp, Warning, TEXT("YLen: %f"), YLength);
	//UE_LOG(LogTemp, Warning, TEXT("ZLen: %f"), ZLength);

	////UE_LOG(LogTemp, Warning, TEXT("Theta: %f"), FGenericPlatformMath::Tan(ZLength / YLength));

	//NewRotator.Roll = FMath::RadiansToDegrees(FGenericPlatformMath::Tan(ZLength / YLength));

	//if (NewRotator.Roll > 90.0f) {
	//	NewRotator.Roll = 90.0f;
	//}
	//else if (NewRotator.Roll < 0.0f) {
	//	NewRotator.Roll = 0.0f;
	//}

	Item->SetWorldRotation(MotionTracker->GetComponentRotation(), true, (FHitResult*)nullptr, ETeleportType::None);

}

void AItem::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);

	if (Held) {
		
		FVector  DevicePosition;
		FRotator DeviceRotation;

		UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(DeviceRotation, DevicePosition);
		
		UpdatePosition(DevicePosition);

		if (!Item->GetBodyInstance()->bLockRotation) {
			UpdateRotation(DeviceRotation);
		}

		if (FVector::Dist(MotionTracker->GetComponentLocation(), GrabSpot->GetComponentLocation()) > 250.0f) {
			SelfDrop();
		}

		//false, (FHitResult*)nullptr, ETeleportType::TeleportPhysics
		//FGenericPlatformMath
	}

}

void AItem::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) {
	if ((Item->GetPhysicsLinearVelocity() - LastVelocity).Size() > 150) {
		LastVelocity = Item->GetPhysicsLinearVelocity();
		if (OnHitSound) {
			UComplexAudioComponent* HitSound = GenerateOnHitSound();
			HitSound->Play();
		}
	}
}

UComplexAudioComponent* AItem::GenerateOnHitSound() {
	UComplexAudioComponent* HitSoundComponent = NewObject<UComplexAudioComponent>(Item, FName("DynamicSound"));
	HitSoundComponent->bAutoDestroy = true;
	HitSoundComponent->bAdvancedOcclusion = true;
	HitSoundComponent->bAmbientSound = this->bAmbientSound;
	HitSoundComponent->ProjectedVolume = this->ProjectedVolume;
	HitSoundComponent->Radius = 50;
	HitSoundComponent->SetupAttachment(Item);
	HitSoundComponent->RegisterComponent();
	HitSoundComponent->SetAttenuationSettings(DefaultAttenuation);
	HitSoundComponent->SetSound(OnHitSound);
	return HitSoundComponent;
}

UStaticMeshComponent* AItem::GetItem() {
	return Item;
}
