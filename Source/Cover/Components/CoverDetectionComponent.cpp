// Fill out your copyright notice in the Description page of Project Settings.


#include "../Components/CoverDetectionComponent.h"
#include "Components/CapsuleComponent.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/Character.h"
#include "../Subsystems/DebugSubsystem.h"
#include "../CoverTypes.h"
#include "../Utils/CVTraceUtils.h"

void UCoverDetectionComponent::BeginPlay()
{
	Super::BeginPlay();
	checkf(GetOwner()->IsA<ACharacter>(), TEXT("UCoverDetectionComponent::BeginPlay() only a Character can use UCoverDetectionComponent"));
	CachedCharacterOwner = StaticCast<ACharacter*>(GetOwner());
}


void UCoverDetectionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UCoverDetectionComponent::DetectCover(OUT FCoverDescription& CoverDescription)
{
	UCapsuleComponent* OwnerCapsuleComponent = CachedCharacterOwner->GetCapsuleComponent();
	// Orig capsule size to fix water case
	UCapsuleComponent* DefaultCapsuleComponent = CachedCharacterOwner->GetClass()->GetDefaultObject<ACharacter>()->GetCapsuleComponent();

	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(GetOwner());

#if ENABLE_DRAW_DEBUG
	UDebugSubsystem* DebugSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UDebugSubsystem>();
	bool bIsDebugEnabled = DebugSubsystem->IsCategoryEnabled(DebugCategoryCoverDetection);
#else
	bool bIsDebugEnabled = false;
#endif

	float DrawTime = 2.0f;

	float BottomZOffset = 2.0f;
	FVector CharacterBottom = CachedCharacterOwner->GetActorLocation() - (OwnerCapsuleComponent->GetScaledCapsuleHalfHeight() - BottomZOffset) * FVector::UpVector;

	// 1. Forward check
	float ForwardTraceCapsuleRadius = OwnerCapsuleComponent->GetScaledCapsuleRadius();
	float ForwardTraceCapsuleHalfHeight = (MaximumCoverHeight - MinimumCoverHeight) * 0.5f;
	// float ForwardTraceCapsuleHalfHeight = (MaximumLedgeHeight - MinimumLedgeHeight) * OwnerCapsuleComponent->GetScaledCapsuleHalfHeight();

	FHitResult ForwardTraceHitResult;
	FVector ForwardTraceStartLocation = CharacterBottom + (MinimumCoverHeight + ForwardTraceCapsuleHalfHeight) * FVector::UpVector;
	FVector ForwardTraceEndLocation = ForwardTraceStartLocation + CachedCharacterOwner->GetActorForwardVector() * ForwardTraceDistance;


	if (!CVTraceUtils::SweepCapsuleSingleByChannel(GetWorld(), ForwardTraceHitResult, ForwardTraceStartLocation, ForwardTraceEndLocation, ForwardTraceCapsuleRadius, ForwardTraceCapsuleHalfHeight, FQuat::Identity, ECC_Covering, QueryParams, FCollisionResponseParams::DefaultResponseParam, bIsDebugEnabled, DrawTime))
	{
		return false;
	}

	// 2. Downward check
	FHitResult DownwardTraceHitResult;
	float DownwardTraceSphereRadius = OwnerCapsuleComponent->GetScaledCapsuleRadius();
	float DownwardTraceDepthOffset = 10.0f;
	FVector DownwardTraceStartLocation = ForwardTraceHitResult.ImpactPoint - ForwardTraceHitResult.ImpactNormal * DownwardTraceDepthOffset;
	DownwardTraceStartLocation.Z = CharacterBottom.Z + MaximumCoverHeight + DownwardTraceSphereRadius;
	FVector DownwardTraceEndLocation(DownwardTraceStartLocation.X, DownwardTraceStartLocation.Y, CharacterBottom.Z);

	if (!CVTraceUtils::SweepSphereSingleByChannel(GetWorld(), DownwardTraceHitResult, DownwardTraceStartLocation, DownwardTraceEndLocation, DownwardTraceSphereRadius, ECC_Covering, QueryParams, FCollisionResponseParams::DefaultResponseParam, bIsDebugEnabled, DrawTime))
	{
		return false;
	}

	CoverDescription.ForwardRotation = (ForwardTraceHitResult.ImpactNormal * FVector(-1.0f, -1.0f, 0.0f)).ToOrientationRotator();
	
	CoverDescription.ForwardHitComponent = ForwardTraceHitResult.GetComponent();
	CoverDescription.DownwardHitComponent = DownwardTraceHitResult.GetComponent();

	CoverDescription.ForwardImpactNormal = ForwardTraceHitResult.ImpactNormal;
	CoverDescription.DownwardImpactNormal = ForwardTraceHitResult.ImpactNormal;
	
	CoverDescription.ForwardImpactPoint = ForwardTraceHitResult.ImpactPoint;
	CoverDescription.DownwardImpactPoint = DownwardTraceHitResult.ImpactPoint;

	return true;
}
