// Fill out your copyright notice in the Description page of Project Settings.


#include "../Characters/CVBaseCharacter.h"

// Sets default values
ACVBaseCharacter::ACVBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

