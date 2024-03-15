// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "../Components/MovementComponents/CVBaseCharacterMovementComponent.h"
#include "CVBaseCharacter.generated.h"

USTRUCT(BlueprintType)
struct FCoveringSettings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimMontage* CoveringMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UCurveVector* CoveringCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float AnimationCorrectionXY = 65.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float AnimationCorrectionZ = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MaxHeight = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MinHeight = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MaxHeightStartTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MinHeightStartTime = 0.5f;
};

class UCVBaseCharacterMovementComponent;

UCLASS()
class COVER_API ACVBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACVBaseCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Movement")
	class UCoverDetectionComponent* CoverDetectionComponent;

	UCVBaseCharacterMovementComponent* CVBaseCharacterMovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Movement | Covering")
	FCoveringSettings HighCoverSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Movement | Covering")
	FCoveringSettings LowCoverSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Movement | Covering", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float LowCoverMaxHeight = 125.0f;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	FORCEINLINE UCVBaseCharacterMovementComponent* GetBaseCharacterMovementComponent() const { return CVBaseCharacterMovementComponent; }
	
	virtual void MoveRight(float Value);
	virtual void MoveForward(float Value);
	virtual void TryCover();

	void SlideCoverRight(float Value);

private:
	const FCoveringSettings& GetCoveringSettings(float LedgeHeight) const;
	FCoveringMovementParameters CoveringParameters;


};
