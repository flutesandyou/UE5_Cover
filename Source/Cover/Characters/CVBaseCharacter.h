// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CVBaseCharacter.generated.h"

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	FORCEINLINE UCVBaseCharacterMovementComponent* GetBaseCharacterMovementComponent() const { return CVBaseCharacterMovementComponent; }

	virtual void TakeCover();
	virtual void StartTakeCover();

private:
	bool bWantsToTakeCover = false;
};
