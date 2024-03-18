// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CVBaseCharacterMovementComponent.generated.h"

struct FMovementCoverDescription
{
	UPrimitiveComponent* ForwardHitComponent = nullptr;
	UPrimitiveComponent* DownwardHitComponent = nullptr;

	FVector ForwardImpactNormal;
	FVector DownwardImpactNormal;

	FVector ForwardImpactPoint;
	FVector DownwardImpactPoint;
};

UENUM(BlueprintType)
enum class ECustomMovementMode : uint8
{
	CMOVE_None = 0 UMETA(DisplayName = "None"),
	CMOVE_TakeCover UMETA(DisplayName = "Covering"),
	CMOVE_InCover UMETA(DisplayName = "InCover"),
	CMOVE_Max UMETA(Hidden)
};

UCLASS()
class COVER_API UCVBaseCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	void AttachToCover(const FMovementCoverDescription& CoveringParameters);
	void DetachFromCover();
	bool IsTakeCover() const;
	bool IsInCover() const;
	bool IsLowCover() const { return bIsLowCover; }


protected:
	virtual void PhysCustom(float DeltaTime, int32 Iterations) override;
	void PhysTakeCover(float DeltaTime, int32 Iterations);

	void PhysInCover(float DeltaTime, int32 Iterations);
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	UPROPERTY(Category = "Character movement: Cover", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float SlideCoverSpeed = 200.0f;

	UPROPERTY(Category = "Character Movement: Cover", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float CoveringBreakingDeceleration = 2048.0f;

private:
	FMovementCoverDescription CurrentCoverDescription;
	FTimerHandle CoveringTimer;
	bool bIsReachedCover = false;
	bool bIsLowCover = false;
};
