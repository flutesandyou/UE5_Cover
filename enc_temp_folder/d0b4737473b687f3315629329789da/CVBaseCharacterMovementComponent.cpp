// Fill out your copyright notice in the Description page of Project Settings.


#include "../MovementComponents/CVBaseCharacterMovementComponent.h"
#include "Curves/CurveVector.h"
#include "../../Characters/CVBaseCharacter.h"
#include "Components/CapsuleComponent.h"

void UCVBaseCharacterMovementComponent::AttachToCover(const FMovementCoverDescription& CoveringParameters)
{
	CurrentCoverDescription = CoveringParameters;

    float MiddleHeight = GetCharacterOwner()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.0f;
    if (CurrentCoverDescription.DownwardImpactPoint.Z < MiddleHeight)
    {
        bIsLowCover = true;
    }
    else
    {
        bIsLowCover = false;
    }
	SetMovementMode(MOVE_Custom, (uint8)ECustomMovementMode::CMOVE_TakeCover);
}

void UCVBaseCharacterMovementComponent::DetachFromCover()
{
	SetMovementMode(MOVE_Walking);
}

bool UCVBaseCharacterMovementComponent::IsTakeCover() const
{
	return UpdatedComponent && MovementMode == MOVE_Custom && CustomMovementMode == (uint8)ECustomMovementMode::CMOVE_TakeCover;
}

bool UCVBaseCharacterMovementComponent::IsInCover() const
{
    return UpdatedComponent && MovementMode == MOVE_Custom && CustomMovementMode == (uint8)ECustomMovementMode::CMOVE_InCover;
}

void UCVBaseCharacterMovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
{
	switch (CustomMovementMode)
	{
	case (uint8)ECustomMovementMode::CMOVE_TakeCover:
	{
		PhysTakeCover(DeltaTime, Iterations);
		break;
	}
	case (uint8)ECustomMovementMode::CMOVE_InCover:
	{
		PhysInCover(DeltaTime, Iterations);
		break;
	}
	default:
		break;
	}

	Super::PhysCustom(DeltaTime, Iterations);
}

void UCVBaseCharacterMovementComponent::PhysTakeCover(float DeltaTime, int32 Iterations)
{
    if (!bIsReachedCover)
    {
        // Calculate rotation
        FVector Normal = CurrentCoverDescription.ForwardImpactNormal;

        // Calculate the rotation to align the character with the surface normal
        FRotator TargetRotation = Normal.Rotation();

        // Ensure the character is rotated correctly by adding a constant offset
        TargetRotation.Yaw -= 180.0f; // Adjusted to -180.0f

        // Calculate offset amount and location
        float OffsetAmount = 50.0f;
        FVector ImpactPoint = CurrentCoverDescription.ForwardImpactPoint;
        ImpactPoint.Z = GetCharacterOwner()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
        FVector TargetPosition = ImpactPoint + (CurrentCoverDescription.ForwardImpactNormal * OffsetAmount);

        // Calculate the delta movement and rotation
        FVector DeltaPosition = TargetPosition - GetOwner()->GetActorLocation();
        FRotator DeltaRotation = (TargetRotation - GetOwner()->GetActorRotation()).GetNormalized();

        float CoverMovementSpeed = 200.0f;
        // Scale the delta movement by DeltaTime to make the movement smooth
        DeltaPosition.Normalize(); // Normalize the delta position vector to ensure constant speed
        DeltaPosition *= CoverMovementSpeed * DeltaTime; // Use a constant speed for movement

        float RotationInterpSpeed = 10.0f;
        // Interpolate between the current rotation and the target rotation
        FRotator NewRotation = FMath::RInterpTo(GetOwner()->GetActorRotation(), TargetRotation, DeltaTime, RotationInterpSpeed);

        // Apply the delta movement gradually
        FHitResult HitResult;
        SafeMoveUpdatedComponent(DeltaPosition, NewRotation, true, HitResult);

        // Check if the character has reached the target position
        FVector CurrentLocation = GetOwner()->GetActorLocation();
        float DistanceToTarget = FVector::Distance(CurrentLocation, TargetPosition);
        float AcceptableDistanceThreshold = 2.0f; // Adjust as needed

        if (DistanceToTarget <= AcceptableDistanceThreshold)
        {
            // Character has reached the target position, detach from cover mode
            bIsReachedCover = true;
            SetMovementMode(MOVE_Custom, (uint8)ECustomMovementMode::CMOVE_InCover);
        }
    }
}


void UCVBaseCharacterMovementComponent::PhysInCover(float DeltaTime, int32 Iterations)
{
	CalcVelocity(DeltaTime, 1.0f, false, CoveringBreakingDeceleration);
	FVector Delta = Velocity * DeltaTime;

	FHitResult HitResult;
	SafeMoveUpdatedComponent(Delta, GetOwner()->GetActorRotation(), true, HitResult);
}

void UCVBaseCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
	if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == (uint8)ECustomMovementMode::CMOVE_InCover)
	{
		ACharacter* DefaultCharacter = CharacterOwner->GetClass()->GetDefaultObject<ACharacter>();
		CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleRadius(), DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight(), true);
	}
    if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == (uint8)ECustomMovementMode::CMOVE_TakeCover)
    {
        bIsReachedCover = false;
    }
}


