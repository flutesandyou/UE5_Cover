// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CVBaseCharacterMovementComponent.generated.h"

struct FCoveringMovementParameters
{
	FVector InitialLocation = FVector::ZeroVector;
	FRotator InitialRotation = FRotator::ZeroRotator;
	UPrimitiveComponent* HitComponent = nullptr;
	FVector TargetLocation = FVector::ZeroVector;
	FRotator TargetRotation = FRotator::ZeroRotator;

	FVector InitialAnimationLocation = FVector::ZeroVector;
	FVector Normal = FVector::ZeroVector;

	float Duration = 1.0f;
	float StartTime = 0.0f;

	UCurveVector* CoveringCurve = nullptr;
};

UENUM(BlueprintType)
enum class ECustomMovementMode : uint8
{
	CMOVE_None = 0 UMETA(DisplayName = "None"),
	CMOVE_Covering UMETA(DisplayName = "Covering"),
	CMOVE_Max UMETA(Hidden)
};

UCLASS()
class COVER_API UCVBaseCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	void AttachToCover(const FCoveringMovementParameters& CoveringParameters);
	void DetachFromCover();
	bool IsCovering() const;


protected:
	virtual void PhysCustom(float DeltaTime, int32 Iterations) override;
	void PhysCovering(float DeltaTime, int32 Iterations);
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	UPROPERTY(Category = "Character movement: Cover", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float SlideCoverSpeed = 200.0f;

	UPROPERTY(Category = "Character Movement: Cover", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float CoveringBreakingDeceleration = 2048.0f;

private:
	FCoveringMovementParameters CurrentCoveringParameters;
	FTimerHandle CoveringTimer;
};
