// Created by Artyom Volkov during the UE4 course


#include "Components/PlayerArmsComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UPlayerArmsComponent::UPlayerArmsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UPlayerArmsComponent::BeginPlay()
{
	Super::BeginPlay();
	InitialRotation = GetRelativeRotation();
	InitialLocation = GetRelativeLocation();
	PlayerMovementComponent = GetOwner()->FindComponentByClass<UCharacterMovementComponent>();
}

void UPlayerArmsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ProcessSwayRotation(DeltaTime);

	if (PlayerMovementComponent)
	{
		PlayerMovementComponent->IsFalling() ? SetJumpOffsetLocation() : RecoverLocation();
	}
}

void UPlayerArmsComponent::SetHorizontalSway(const float AxisValue)
{
	FRotator FinalRotation = GetRelativeRotation();
	FinalRotation.Yaw = FinalRotation.Yaw - AxisValue * SwayPower;
	SetRelativeRotation(FinalRotation);
}

void UPlayerArmsComponent::SetVerticalSway(const float AxisValue)
{
	FRotator FinalRotation = GetRelativeRotation();
	FinalRotation.Roll = FinalRotation.Roll - AxisValue * SwayPower;
	SetRelativeRotation(FinalRotation);
}

void UPlayerArmsComponent::ProcessSwayRotation(const float DeltaTime)
{
	const FRotator TargetRotation = FRotator(GetRelativeRotation().Pitch,
	                                         InitialRotation.Yaw,
	                                         InitialRotation.Roll);
	const FRotator FinalRotation = FMath::RInterpTo(GetRelativeRotation(),
	                                                TargetRotation,
	                                                DeltaTime,
	                                                SwaySpeed);
	SetRelativeRotation(FinalRotation);
}

void UPlayerArmsComponent::SetJumpOffsetLocation()
{
	const FVector NewLocation = FMath::Lerp(GetRelativeLocation(), InitialLocation + JumpLocationOffset, JumpLerpSpeed);
	SetRelativeLocation(NewLocation);
}

void UPlayerArmsComponent::RecoverLocation()
{
	const FVector NewLocation = FMath::Lerp(GetRelativeLocation(), InitialLocation, RecoverLerpSpeed);
	SetRelativeLocation(NewLocation);
}

