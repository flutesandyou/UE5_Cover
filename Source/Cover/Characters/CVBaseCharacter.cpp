// Fill out your copyright notice in the Description page of Project Settings.


#include "../Characters/CVBaseCharacter.h"
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
		bIsInCover = false;
		bMovedRight = true;
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
			bIsInCover = true;
		}		
	}
	return;
}

float ACVBaseCharacter::GetRightMovementSpeed()
{
	return GetVelocity().Dot(GetActorRightVector());
}

const FCoveringSettings& ACVBaseCharacter::GetCoveringSettings(float CoverHeight) const
{
	return CoverHeight > LowCoverMaxHeight ? HighCoverSettings : LowCoverSettings;
}

void ACVBaseCharacter::MoveRight(float Value)
{
	if (GetCharacterMovement()->IsMovingOnGround() && !GetBaseCharacterMovementComponent()->IsInCover() && !FMath::IsNearlyZero(Value, 1e-6f) || GetCharacterMovement()->IsFalling())
	{
		FRotator YawRotator(0.0f, GetControlRotation().Yaw, 0.0f);
		FVector RightVector = YawRotator.RotateVector(FVector::RightVector);
		AddMovementInput(RightVector, Value);
	}

	if (GetBaseCharacterMovementComponent()->IsInCover() && !FMath::IsNearlyZero(Value))
	{
		// Calculate the direction along the cover surface
		FVector CharacterToNormal = FVector::CrossProduct(MovementCoverDescription.ForwardImpactNormal, GetActorUpVector()).GetSafeNormal2D();

		// Get the character's right vector
		FVector CharacterRight = GetActorRightVector(); 

		// Calculate the dot product between character's right vector and CharacterToNormal
		float DotProduct = FVector::DotProduct(CharacterRight, CharacterToNormal);


		if (Value > 0.0f)
		{
			bMovedRight = true;
			if (!CoverDetectionComponent->UpdateCoverSide(FName("Right")))	
			{
				// If TraceBool is false and Value is positive, set Value to 0
				Value = 0.0f;
			}
			if (CoverDetectionComponent->UpdateCoverSide(FName("Right")))
			{
				CoverDetectionComponent->UpdateCover(UpdateCoverDescription);
				GetBaseCharacterMovementComponent()->AddInputVector(UpdateCoverDescription.Direction, false);
				GetBaseCharacterMovementComponent()->AddInputVector(GetActorForwardVector(), false);
			}
		}


		if (Value < 0.0f)
		{
			bMovedRight = false;
			if (!CoverDetectionComponent->UpdateCoverSide(FName("Left")))
			{
				// If TraceBool is false and Value is positive, set Value to 0
				Value = 0.0f;
			}
			if (CoverDetectionComponent->UpdateCoverSide(FName("Left")))
			{
				CoverDetectionComponent->UpdateCover(UpdateCoverDescription);
				GetBaseCharacterMovementComponent()->AddInputVector(-UpdateCoverDescription.Direction, false);
				GetBaseCharacterMovementComponent()->AddInputVector(GetActorForwardVector(), false);
			}
		}

		FRotator CurrentRotation = GetActorRotation();
		//FRotator CurrentRotation = GetActorForwardVector().ToOrientationRotator();

		FRotator TargetRotation = FRotationMatrix::MakeFromX(UpdateCoverDescription.HitNormal).Rotator();
		TargetRotation.Yaw -= 180.0f;
		float DeltaTime = GetWorld()->GetDeltaSeconds();
		float InterpSpeed = 10.0f; // Adjust this value for smoother or faster interpolation

		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, InterpSpeed);
		SetActorRotation(NewRotation);
	}
}

void ACVBaseCharacter::MoveForward(float Value)
{
	FUpdateCoverDescription CenterUpdateCoverDescription;
	if (GetCharacterMovement()->IsMovingOnGround() && !CVBaseCharacterMovementComponent->IsInCover() && !FMath::IsNearlyZero(Value, 1e-6f) || GetCharacterMovement()->IsFalling())
	{
		FRotator YawRotator(0.0f, GetControlRotation().Yaw, 0.0f);
		FVector ForwardVector = YawRotator.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardVector, Value);
		//AddMovementInput(GetActorForwardVector(), Value);
	}
	if (CVBaseCharacterMovementComponent->IsInCover() && !FMath::IsNearlyZero(Value))
	{
		// If character is covering, move along CharacterToNormal
		FVector CharacterToNormal = FVector::CrossProduct(CenterUpdateCoverDescription.HitNormal, GetActorUpVector());

		// Calculate the dot product between character's forward vector and CharacterToNormal
		float DotProduct = FVector::DotProduct(GetActorForwardVector(), CharacterToNormal.GetSafeNormal2D());

		// Adjust the movement direction based on the dot product
		if (DotProduct < 0)
		{
			// If character is facing away from CharacterToNormal direction, move backward
			AddMovementInput(CharacterToNormal, Value);
		}
		else
		{
			// If character is facing towards CharacterToNormal direction, move forward
			AddMovementInput(CharacterToNormal, -Value);
		}
	}
}

