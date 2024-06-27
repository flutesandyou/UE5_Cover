// Fill out your copyright notice in the Description page of Project Settings.


#include "../MovementComponents/CVBaseCharacterMovementComponent.h"
#include "Curves/CurveVector.h"
#include "../../Characters/CVBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"

void UCVBaseCharacterMovementComponent::AttachToCover(const FMovementCoverDescription& CoveringParameters)
{
	CurrentCoverDescription = CoveringParameters;
    // Get the character's location
    FVector CharacterLocation = GetOwner()->GetActorLocation();
    

    float MiddleHeight = CrouchedHalfHeight * 2.5f;
    if (CurrentCoverDescription.DownwardImpactPoint.Z < MiddleHeight)
    {
        bIsInLowCover = true;
        GetCharacterOwner()->Crouch();
    }
    else
    {
        bIsInLowCover = false;
    }

	SetMovementMode(MOVE_Custom, (uint8)ECustomMovementMode::CMOVE_TakeCover);
}

void UCVBaseCharacterMovementComponent::DetachFromCover()
{
    if (bIsInCover == true)
    {
        bIsInCover = false;
        bIsInLowCover = false;
    }
}

bool UCVBaseCharacterMovementComponent::IsTakeCover() const
{
	return UpdatedComponent && MovementMode == MOVE_Custom && CustomMovementMode == (uint8)ECustomMovementMode::CMOVE_TakeCover;
}

bool UCVBaseCharacterMovementComponent::IsInCover() const
{
    return bIsInCover;
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
	default:
		break;
	}

	Super::PhysCustom(DeltaTime, Iterations);
}

void UCVBaseCharacterMovementComponent::PhysTakeCover(float DeltaTime, int32 Iterations)
{
    if (bIsInCover) return;

    UWorld* World = GetWorld();
    if (!World) return;

    // Calculate the surface normal and desired rotation
    FVector Normal = CurrentCoverDescription.ForwardImpactNormal;
    FRotator TargetRotation = (Normal * -1.0f).Rotation();

    // Calculate the target position
    float CapsuleRadius = GetCharacterOwner()->GetCapsuleComponent()->GetUnscaledCapsuleRadius();
    FVector ImpactPoint = CurrentCoverDescription.ForwardImpactPoint;
    ImpactPoint.Z = GetCharacterOwner()->GetCapsuleComponent()->GetComponentLocation().Z;
    FVector TargetPosition = ImpactPoint + (Normal * CapsuleRadius);

    // Calculate movement and rotation deltas
    FVector CurrentLocation = GetOwner()->GetActorLocation();
    FVector DeltaPosition = TargetPosition - CurrentLocation;
    DeltaPosition = DeltaPosition.GetSafeNormal() * 200.0f * DeltaTime;

    FRotator NewRotation = FMath::RInterpTo(GetOwner()->GetActorRotation(), TargetRotation, DeltaTime, 10.0f);

    FVector DistanceToCover = TargetPosition - CurrentLocation;
    float Distance = DistanceToCover.Size() / 2.0f;

    // Initialize the timer if it hasn't been started yet
    if (!bIsCoverTimerStarted)
    {
        CoverStartTime = World->GetTimeSeconds();
        CoverDuration = Distance * DeltaTime; // Set the duration based on your requirement
        bIsCoverTimerStarted = true;
    }

    // Move the character
    FHitResult HitResult;
    SafeMoveUpdatedComponent(DeltaPosition, NewRotation, true, HitResult);

    // Draw debug sphere and line
    float SphereRadius = 50.0f; // Adjust sphere radius as needed
    DrawDebugSphere(World, TargetPosition, SphereRadius, 12, FColor::Green, false, 2.0f);
    DrawDebugLine(World, TargetPosition, CurrentLocation, FColor::Yellow, false, 2.0f);

    // Check if the duration has elapsed
    float ElapsedTime = World->GetTimeSeconds() - CoverStartTime;
    if (ElapsedTime >= CoverDuration)
    {
        bIsInCover = true;
        SetMovementMode(MOVE_Walking);
        bIsCoverTimerStarted = false; // Reset the timer for the next use
    }
}



void UCVBaseCharacterMovementComponent::PhysInCover(float DeltaTime, int32 Iterations)
{

}

void UCVBaseCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

    if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == (uint8)ECustomMovementMode::CMOVE_TakeCover)
    {
    }
    if (PreviousMovementMode == MOVE_Walking)
    {
    }
}

float UCVBaseCharacterMovementComponent::GetMaxSpeed() const
{
    float Result = Super::GetMaxSpeed();
    if (bIsInCover)
    {
        Result = InCoverSpeed;
    }
    return Result;
}