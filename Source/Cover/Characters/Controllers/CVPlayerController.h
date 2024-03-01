// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../../Characters/CVBaseCharacter.h"
#include "CVPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class COVER_API ACVPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetPawn(APawn* InPawn) override;

protected:
	virtual void SetupInputComponent() override;

private:
	void TakeCover();

	TSoftObjectPtr<ACVBaseCharacter> CachedBaseCharacter;
};
