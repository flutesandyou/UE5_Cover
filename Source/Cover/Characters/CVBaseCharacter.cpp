// Fill out your copyright notice in the Description page of Project Settings.


#include "../Characters/CVBaseCharacter.h"
#include "../Components/CoverDetectionComponent.h"
#include "Curves/CurveVector.h"
#include "UObject/ConstructorHelpers.h"
#include <Kismet/KismetMathLibrary.h>

// Sets default values
ACVBaseCharacter::ACVBaseCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UCVBaseCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	CVBaseCharacterMovementComponent = StaticCast<UCVBaseCharacterMovementComponent*>(GetCharacterMovement());

	CoverDetectionComponent = CreateDefaultSubobject<UCoverDetectionComponent>(TEXT("CoverDetection"));
}

// Called when the game starts or when spawned
void ACVBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACVBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetBaseCharacterMovementComponent()->IsInCover())
	{
		FRotator Rotation = FRotationMatrix::MakeFromX(MovementCoverDescription.ForwardImpactNormal).Rotator();
		Rotation.Yaw -= 180.0f; // Adjusted to -180.0f
		SetActorRotation(Rotation);
		CoverDetectionComponent->UpdateCover();
	}
}

// Called to bind functionality to input
void ACVBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void ACVBaseCharacter::TryCover()
{
	if (GetBaseCharacterMovementComponent()->IsInCover())
	{
		GetBaseCharacterMovementComponent()->DetachFromCover();
	}
	else if (!GetBaseCharacterMovementComponent()->IsInCover())
	{
		FCoverDescription CoverDescription;
		if (CoverDetectionComponent->DetectCover(CoverDescription))
		{
			//TODO activate Taking Cover
			MovementCoverDescription.ForwardHitComponent = CoverDescription.ForwardHitComponent;
			MovementCoverDescription.DownwardHitComponent = CoverDescription.DownwardHitComponent;

			MovementCoverDescription.ForwardImpactNormal = CoverDescription.ForwardImpactNormal;
			MovementCoverDescription.DownwardImpactNormal = CoverDescription.DownwardImpactNormal;

			MovementCoverDescription.ForwardImpactPoint = CoverDescription.ForwardImpactPoint;
			MovementCoverDescription.DownwardImpactPoint = CoverDescription.DownwardImpactPoint;

			GetBaseCharacterMovementComponent()->AttachToCover(MovementCoverDescription);
		}		
	}
	return;
}

void ACVBaseCharacter::SlideCoverRight(float Value)
{
	if (GetBaseCharacterMovementComponent()->IsInCover() && !FMath::IsNearlyZero(Value))
	{
		// Calculate the direction along the cover surface
		FVector CharacterToNormal = FVector::CrossProduct(MovementCoverDescription.ForwardImpactNormal, GetActorUpVector()).GetSafeNormal2D();

		// Get the character's right vector
		FVector CharacterRight = GetActorRightVector();

		// Calculate the dot product between character's right vector and CharacterToNormal
		float DotProduct = FVector::DotProduct(CharacterRight, CharacterToNormal);

		// Adjust the movement direction based on the dot product
		if (DotProduct > 0)
		{
			// If character's right vector aligns with the cover surface direction, move right along the cover surface
			AddMovementInput(CharacterToNormal, Value);
		}
		else
		{
			// If character's right vector aligns opposite to the cover surface direction, move left along the cover surface
			AddMovementInput(CharacterToNormal, -Value);
		}
	}
}

const FCoveringSettings& ACVBaseCharacter::GetCoveringSettings(float CoverHeight) const
{
	return CoverHeight > LowCoverMaxHeight ? HighCoverSettings : LowCoverSettings;
}

void ACVBaseCharacter::MoveRight(float Value)
{
	if (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling() || CVBaseCharacterMovementComponent->IsInCover() && !FMath::IsNearlyZero(Value, 1e-6f))
	{
		FRotator YawRotator(0.0f, GetControlRotation().Yaw, 0.0f);
		FVector RightVector = YawRotator.RotateVector(FVector::RightVector);
		AddMovementInput(RightVector, Value);
	}
}

void ACVBaseCharacter::MoveForward(float Value)
{
	if (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling() || !CVBaseCharacterMovementComponent->IsInCover() && !FMath::IsNearlyZero(Value, 1e-6f))
	{
		FRotator YawRotator(0.0f, GetControlRotation().Yaw, 0.0f);
		FVector ForwardVector = YawRotator.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardVector, Value);
		//AddMovementInput(GetActorForwardVector(), Value);
	}
	else if (CVBaseCharacterMovementComponent->IsInCover())
	{
		// If character is covering, move along CharacterToNormal
		FVector CharacterToNormal = FVector::CrossProduct(MovementCoverDescription.ForwardImpactNormal, GetActorUpVector());

		// Calculate the dot product between character's forward vector and CharacterToNormal
		float DotProduct = FVector::DotProduct(GetActorForwardVector(), CharacterToNormal.GetSafeNormal2D());

		// Adjust the movement direction based on the dot product
		if (DotProduct < 0)
		{
			// If character is facing away from CharacterToNormal direction, move backward
			AddMovementInput(CharacterToNormal, -Value);
		}
		else
		{
			// If character is facing towards CharacterToNormal direction, move forward
			AddMovementInput(CharacterToNormal, Value);
		}
	}
}
