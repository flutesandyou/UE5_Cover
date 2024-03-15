// Fill out your copyright notice in the Description page of Project Settings.


#include "../MovementComponents/CVBaseCharacterMovementComponent.h"
#include "Curves/CurveVector.h"
#include "../../Characters/CVBaseCharacter.h"
#include "Components/CapsuleComponent.h"

void UCVBaseCharacterMovementComponent::AttachToCover(const FCoveringMovementParameters& CoveringParameters)
{
	CurrentCoveringParameters = CoveringParameters;

	FVector Normal = CurrentCoveringParameters.Normal;
	FVector UpDirection(0.0f, 0.0f, 1.0f);

	FVector RotationAxis = FVector::CrossProduct(Normal, UpDirection);
	RotationAxis.Normalize();

	// Convert the rotation axis and angle to a FRotator
	FRotator Rotation = FRotator(RotationAxis.Rotation());
	Rotation.Yaw += -90.0f;

	GetCharacterOwner()->SetActorRotation(Rotation);
	SetMovementMode(MOVE_Custom, (uint8)ECustomMovementMode::CMOVE_Covering);
}

void UCVBaseCharacterMovementComponent::DetachFromCover()
{
	SetMovementMode(MOVE_Walking);
}

bool UCVBaseCharacterMovementComponent::IsCovering() const
{
	return UpdatedComponent && MovementMode == MOVE_Custom && CustomMovementMode == (uint8)ECustomMovementMode::CMOVE_Covering;
}

void UCVBaseCharacterMovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
{
	switch (CustomMovementMode)
	{
	case (uint8)ECustomMovementMode::CMOVE_Covering:
	{
		PhysCovering(DeltaTime, Iterations);
		break;
	}
	default:
		break;
	}

	Super::PhysCustom(DeltaTime, Iterations);
}

void UCVBaseCharacterMovementComponent::PhysCovering(float DeltaTime, int32 Iterations)
{
	CalcVelocity(DeltaTime, 1.0f, false, CoveringBreakingDeceleration);
	FVector Delta = Velocity * DeltaTime;

	FHitResult HitResult;
	SafeMoveUpdatedComponent(Delta, GetOwner()->GetActorRotation(), true, HitResult);

	//float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(CoveringTimer) + CurrentCoveringParameters.StartTime;
	//FVector CoveringCurveValue = CurrentCoveringParameters.CoveringCurve->GetVectorValue(ElapsedTime);

	//float PositionAlpha = CoveringCurveValue.X;
	//float XYCorrectionAlpha = CoveringCurveValue.Y;
	//float ZCorrectionAlpha = CoveringCurveValue.Z;s

	//FVector CorrectedInitialLocation = FMath::Lerp(CurrentCoveringParameters.InitialLocation, CurrentCoveringParameters.InitialAnimationLocation, XYCorrectionAlpha);
	//CorrectedInitialLocation.Z = FMath::Lerp(CurrentCoveringParameters.InitialLocation.Z, CurrentCoveringParameters.InitialAnimationLocation.Z, ZCorrectionAlpha);

	//FVector NewLocation = FMath::Lerp(CorrectedInitialLocation, CurrentCoveringParameters.TargetLocation, PositionAlpha);
	//FRotator NewRotation = FMath::Lerp(CurrentCoveringParameters.InitialRotation, CurrentCoveringParameters.TargetRotation, PositionAlpha);

	//NewLocation += CurrentCoveringParameters.HitComponent->GetComponentLocation();
	//NewRotation += CurrentCoveringParameters.HitComponent->GetComponentRotation();

	//FVector Delta = NewLocation - GetActorLocation();

	//FHitResult HitResult;
	//SafeMoveUpdatedComponent(Delta, NewRotation, false, HitResult);
}

void UCVBaseCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
	if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == (uint8)ECustomMovementMode::CMOVE_Covering)
	{
		ACharacter* DefaultCharacter = CharacterOwner->GetClass()->GetDefaultObject<ACharacter>();
		CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleRadius(), DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight(), true);
	}
}
