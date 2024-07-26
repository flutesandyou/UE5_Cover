// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoverDetectionComponent.generated.h"

USTRUCT(BlueprintType)
struct FUpdateCoverDescription
{
	GENERATED_BODY()
	FVector HitNormal;
	FVector Direction;
	FHitResult HitResult;
};

USTRUCT(BlueprintType)
struct FCoverDescription
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cover description")
	FVector Location;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cover description")
	FRotator ForwardRotation;

	FVector ForwardImpactNormal;
	FVector DownwardImpactNormal;

	FVector ForwardImpactPoint;
	FVector DownwardImpactPoint;

	UPrimitiveComponent* ForwardHitComponent = nullptr;
	UPrimitiveComponent* DownwardHitComponent = nullptr;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COVER_API UCoverDetectionComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Detection settings", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float MinimumCoverHeight = 40.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Detection settings", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float MaximumCoverHeight = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Detection settings", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float ForwardTraceDistance = 200.0f;

private:
	TWeakObjectPtr<class ACharacter> CachedCharacterOwner;

public:	
	bool DetectCover(OUT FCoverDescription& CoverDescription);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);
	bool UpdateCover(const FName& MoveDirection, OUT FUpdateCoverDescription& UpdateCoverDescription);
};
