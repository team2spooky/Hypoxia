// Fill out your copyright notice in the Description page of Project Settings.

#include "Hypoxia.h"
#include "Terminal.h"
#include "Components/TextRenderComponent.h"

void ATerminal::Use() {
	USoundConcurrency* AudioLogConcurrency = LoadObject<USoundConcurrency>(NULL, TEXT("/Game/AudioLogConcurrency.AudioLogConcurrency"), NULL, LOAD_None, NULL);
	// Fire and Forget... will need to change if pausing the audio log is a thing
	UGameplayStatics::PlaySound2D(GetWorld(), AudioLog, 1.0f, 1.0f, 0.0f, AudioLogConcurrency);
	TArray<UTextRenderComponent*> Components;
	GetComponents<UTextRenderComponent>(Components);
	Components[0]->SetText("Played");
}
