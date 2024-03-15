// Fill out your copyright notice in the Description page of Project Settings.


#include "../Characters/CVBaseCharacter.h"
#include "../Components/CoverDetectionComponent.h"
#include "Curves/CurveVector.h"
#include "UObject/ConstructorHelpers.h"

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
	if (GetBaseCharacterMovementComponent()->IsCovering())
	{
		GetBaseCharacterMovementComponent()->DetachFromCover();
	}
	else if (!GetBaseCharacterMovementComponent()->IsCovering())
	{
		FCoverDescription CoverDescription;
		if (CoverDetectionComponent->DetectCover(CoverDescription))
		{
			//TODO activate Taking Cover

			CoveringParameters.InitialLocation = GetActorLocation() - CoverDescription.HitComponent->GetComponentLocation();
			CoveringParameters.InitialRotation = GetActorRotation() - CoverDescription.HitComponent->GetComponentRotation();
			CoveringParameters.TargetLocation = CoverDescription.Location - CoverDescription.HitComponent->GetComponentLocation();
			CoveringParameters.TargetRotation = CoverDescription.Rotation - CoverDescription.HitComponent->GetComponentRotation();
			CoveringParameters.HitComponent = CoverDescription.HitComponent;
			CoveringParameters.Normal = CoverDescription.Normal;
			FVector DownwardTraceHitResult = CoverDescription.DownwardTraceHitResult;

			FVector CharacterBottom = GetActorLocation() - GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * FVector::UpVector;

			float CoveringHeight = DownwardTraceHitResult.Z - CharacterBottom.Z;
			const FCoveringSettings& CoveringSettings = GetCoveringSettings(CoveringHeight);

			float MinRange;
			float MaxRange;
			CoveringSettings.CoveringCurve->GetTimeRange(MinRange, MaxRange);

			CoveringParameters.Duration = MaxRange - MinRange;
			CoveringParameters.CoveringCurve = CoveringSettings.CoveringCurve;

			FVector2D SourceRange(CoveringSettings.MinHeight, CoveringSettings.MaxHeight);
			FVector2D TargetRange(CoveringSettings.MinHeightStartTime, CoveringSettings.MaxHeightStartTime);
			CoveringParameters.StartTime = FMath::GetMappedRangeValueClamped(SourceRange, TargetRange, CoveringHeight);

			CoveringParameters.InitialAnimationLocation = CoveringParameters.TargetLocation - CoveringSettings.AnimationCorrectionZ * FVector::UpVector + CoveringSettings.AnimationCorrectionXY * CoverDescription.Normal;

			GetBaseCharacterMovementComponent()->AttachToCover(CoveringParameters);

			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			AnimInstance->Montage_Play(CoveringSettings.CoveringMontage, 1.0f, EMontagePlayReturnType::Duration, CoveringParameters.StartTime);

		}		
	}
	return;
}

void ACVBaseCharacter::SlideCoverRight(float Value)
{
}

const FCoveringSettings& ACVBaseCharacter::GetCoveringSettings(float CoverHeight) const
{
	return CoverHeight > LowCoverMaxHeight ? HighCoverSettings : LowCoverSettings;
}

void ACVBaseCharacter::MoveRight(float Value)
{
	if (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling() || !CVBaseCharacterMovementComponent->IsCovering() && !FMath::IsNearlyZero(Value, 1e-6f))
	{
		FRotator YawRotator(0.0f, GetControlRotation().Yaw, 0.0f);
		FVector RightVector = YawRotator.RotateVector(FVector::RightVector);
		AddMovementInput(RightVector, Value);
	}
	if (GetBaseCharacterMovementComponent()->IsCovering() && !FMath::IsNearlyZero(Value))
	{
		// Calculate the direction along the cover surface
		FVector CharacterToNormal = FVector::CrossProduct(CoveringParameters.Normal, GetActorUpVector()).GetSafeNormal2D();

		// Calculate the dot product between character's right vector and CharacterToNormal
		float DotProduct = FVector::DotProduct(GetActorRightVector(), CharacterToNormal);

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

void ACVBaseCharacter::MoveForward(float Value)
{
	if (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling() || !CVBaseCharacterMovementComponent->IsCovering() && !FMath::IsNearlyZero(Value, 1e-6f))
	{
		FRotator YawRotator(0.0f, GetControlRotation().Yaw, 0.0f);
		FVector ForwardVector = YawRotator.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardVector, Value);
		//AddMovementInput(GetActorForwardVector(), Value);
	}
	else if (CVBaseCharacterMovementComponent->IsCovering())
	{
		// If character is covering, move along CharacterToNormal
		FVector CharacterToNormal = FVector::CrossProduct(CoveringParameters.Normal, GetActorUpVector());

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
