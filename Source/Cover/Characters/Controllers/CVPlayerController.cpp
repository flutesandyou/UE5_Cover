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
	InputComponent->BindAxis("MoveForward/Backwards", this, &ACVPlayerController::MoveForward);
	InputComponent->BindAxis("SlideCoverRight", this, &ACVPlayerController::SlideCoverRight);
	InputComponent->BindAxis("MoveRight/Left", this, &ACVPlayerController::MoveRight);
}

void ACVPlayerController::MoveForward(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->MoveForward(Value);
	}
}
void ACVPlayerController::MoveRight(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->MoveRight(Value);
	}
}
void ACVPlayerController::TakeCover()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->TryCover();
	}
}

void ACVPlayerController::SlideCoverRight(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SlideCoverRight(Value);
	}
}
