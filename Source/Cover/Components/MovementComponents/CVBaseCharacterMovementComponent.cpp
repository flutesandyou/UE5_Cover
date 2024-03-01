// Fill out your copyright notice in the Description page of Project Settings.


#include "../MovementComponents/CVBaseCharacterMovementComponent.h"
#include "Curves/CurveVector.h"

void UCVBaseCharacterMovementComponent::StartMantle(const FCoveringMovementParameters& CoveringParameters)
{
	CurrentCoveringParameters = CoveringParameters;
	SetMovementMode(EMovementMode::MOVE_Custom, (uint8)ECustomMovementMode::CMOVE_Covering);
}

void UCVBaseCharacterMovementComponent::EndMantle()
{
	SetMovementMode(MOVE_Walking);
}

bool UCVBaseCharacterMovementComponent::IsTakingCover() const
{
	//return UpdatedComponent && MovementMode == MOVE_Custom && CustomMovementMode == (uint8)ECustomMovementMode::CMOVE_Covering;
	return false;
}

void UCVBaseCharacterMovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
{
	switch (CustomMovementMode)
	{
	case (uint8)ECustomMovementMode::CMOVE_Covering:
	{
		//float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(CoveringTimer) + CurrentCoveringParameters.StartTime;
		//FVector CoveringCurveValue = CurrentCoveringParameters.CoveringCurve->GetVectorValue(ElapsedTime);

		//float PositionAlpha = CoveringCurveValue.X;
		//float XYCorrectionAlpha = CoveringCurveValue.Y;
		//float ZCorrectionAlpha = CoveringCurveValue.Z;

		//FVector CorrectedInitialLocation = FMath::Lerp(CurrentCoveringParameters.InitialLocation, CurrentCoveringParameters.InitialAnimationLocation, XYCorrectionAlpha);
		//CorrectedInitialLocation.Z = FMath::Lerp(CurrentCoveringParameters.InitialLocation.Z, CurrentCoveringParameters.InitialAnimationLocation.Z, ZCorrectionAlpha);

		//FVector NewLocation = FMath::Lerp(CorrectedInitialLocation, CurrentCoveringParameters.TargetLocation, PositionAlpha);
		//FRotator NewRotation = FMath::Lerp(CurrentCoveringParameters.InitialRotation, CurrentCoveringParameters.TargetRotation, PositionAlpha);

		//NewLocation += CurrentCoveringParameters.HitComponent->GetComponentLocation();
		//NewRotation += CurrentCoveringParameters.HitComponent->GetComponentRotation();

		//FVector Delta = NewLocation - GetActorLocation();

		//FHitResult HitResult;
		//SafeMoveUpdatedComponent(Delta, NewRotation, false, HitResult);
		break;
	}
	default:
		break;
	}

	Super::PhysCustom(DeltaTime, Iterations);
}

void UCVBaseCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	if (MovementMode == MOVE_Custom)
	{
		//switch (CustomMovementMode)
		//{
		//case (uint8)ECustomMovementMode::CMOVE_Covering:
		//{
		//	GetWorld()->GetTimerManager().SetTimer(CoveringTimer, this, &UCVBaseCharacterMovementComponent::EndTakingCover, CurrentCoveringParameters.Duration, false);
		//	break;
		//}
		//default:
		//	break;
		//}
	}
}
