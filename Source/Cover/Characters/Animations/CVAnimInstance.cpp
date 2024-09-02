// Fill out your copyright notice in the Description page of Project Settings.


#include "../Animations/CVAnimInstance.h"
#include "../CVBaseCharacter.h"
#include "../../Components/MovementComponents/CVBaseCharacterMovementComponent.h"

void UCVAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	checkf(TryGetPawnOwner()->IsA<ACVBaseCharacter>(), TEXT("UCVAnimInstance::NativeBeginPlay() UCVAnimInstance can only be used with ACVBaseCharacter"));
	CachedBaseCharacter = StaticCast<ACVBaseCharacter*>(TryGetPawnOwner());
}

void UCVAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!CachedBaseCharacter.IsValid())
	{
		return;
	}
	UCVBaseCharacterMovementComponent* CharacterMovement = CachedBaseCharacter->GetBaseCharacterMovementComponent();
	CharSpeed = CharacterMovement->Velocity.Size();
	bIsCrouching = CharacterMovement->IsCrouching();
	bIsTakeCover = CharacterMovement->IsTakeCover();
	bIsInCover = CharacterMovement->IsInCover();
	bIsInLowCover = CharacterMovement->IsInLowCover();

	if (CachedBaseCharacter->IsPlayerControlled() && GEngine) // Ensure the character is the one you control
	{
		FString DebugMessage = FString::Printf(TEXT("bIsCrouching: %s"), bIsCrouching ? TEXT("True") : TEXT("False"));
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, DebugMessage);
	}

	if (bIsInCover || bIsInLowCover)
	{
		bMovedRight = CachedBaseCharacter->GetMovedRight();
		RightMovementSpeed = CachedBaseCharacter->GetRightMovementSpeed();

		if (GEngine)
		{
			FString DebugMessage = FString::Printf(TEXT("Right Movement Speed: %.2f"), RightMovementSpeed);
			GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, DebugMessage);
		}
	}
}
