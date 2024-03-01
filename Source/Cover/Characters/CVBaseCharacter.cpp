// Fill out your copyright notice in the Description page of Project Settings.


#include "../Characters/CVBaseCharacter.h"
#include "../Components/MovementComponents/CVBaseCharacterMovementComponent.h"
#include "../Components/CoverDetectionComponent.h"

// Sets default values
ACVBaseCharacter::ACVBaseCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UCVBaseCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CoverDetectionComponent = CreateDefaultSubobject<UCoverDetectionComponent>(TEXT("CoverDetection"));
}

// Called when the game starts or when spawned
void ACVBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACVBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACVBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void ACVBaseCharacter::TakeCover()
{
	bWantsToTakeCover = true;
	StartTakeCover();
}

void ACVBaseCharacter::StartTakeCover()
{
	if (bWantsToTakeCover == true && !GetBaseCharacterMovementComponent()->IsTakingCover())
	{
		FCoverDescription CoverDescription;
		if (CoverDetectionComponent->DetectCover(CoverDescription))
		{
			//TODO activate Taking Cover
		}
	}
}

