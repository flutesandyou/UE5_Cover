// Fill out your copyright notice in the Description page of Project Settings.


#include "../Controllers/CVPlayerController.h"

void ACVPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	CachedBaseCharacter = Cast<ACVBaseCharacter>(InPawn);
}

void ACVPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("TakeCover", EInputEvent::IE_Pressed, this, &ACVPlayerController::TakeCover);
}

void ACVPlayerController::TakeCover()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->TakeCover();
	}
}
