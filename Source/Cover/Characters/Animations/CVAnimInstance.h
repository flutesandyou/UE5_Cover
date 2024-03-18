// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CVAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class COVER_API UCVAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsCrouching = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsTakeCover = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsInCover = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsLowCover = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	float CharSpeed = 0.0f;

private:
	TWeakObjectPtr<class ACVBaseCharacter> CachedBaseCharacter;

};
