// Created by Artyom Volkov during the UE4 course


#include "Components/PlayerArmsComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Characters/PlayerCharacter.h"

UPlayerArmsComponent::UPlayerArmsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UPlayerArmsComponent::BeginPlay()
{
	Super::BeginPlay();
	InitialRotation = GetRelativeRotation();
	InitialLocation = GetRelativeLocation();
	PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	PlayerMovementComponent = GetOwner()->FindComponentByClass<UCharacterMovementComponent>();
}

void UPlayerArmsComponent::TickComponent(float DeltaTime,
                                         ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ProcessSwayRotation(DeltaTime);

	if (PlayerMovementComponent)
	{
		FVector ArmsOffset = LocationOffset * UKismetMathLibrary::Quat_UnrotateVector(
			GetOwner()->GetActorRotation().Quaternion(),
			PlayerMovementComponent->Velocity).GetSafeNormal();

		if (ArmsOffset.X == 0 && ArmsOffset.Y == 0)
		{
			ArmsOffset.Z = FMath::Abs(ArmsOffset.Z);
		}

		if (!PlayerMovementComponent->IsFalling() && (!bIsShooting|| !PlayerCharacter->CanShoot()))
		{
			const float Time = GetWorld()->GetTimeSeconds();
			const float NormalizedPlayerSpeed = PlayerMovementComponent->Velocity.GetSafeNormal().Size();
			const float MovementSin = FMath::Sin(NormalizedPlayerSpeed * AnimationFrequency * Time);
			ArmsOffset.X = ArmsOffset.X + NormalizedPlayerSpeed * MovementOffset.X * FMath::Pow(MovementSin, 2);
			ArmsOffset.Y = ArmsOffset.Y + NormalizedPlayerSpeed * MovementOffset.Y * MovementSin;
			ArmsOffset.Z = ArmsOffset.Z + NormalizedPlayerSpeed * MovementOffset.Z * FMath::Pow(MovementSin, 2);
		}


		const FVector NewLocation = FMath::Lerp(GetRelativeLocation(),
		                                        InitialLocation + ArmsOffset,
		                                        RecoverLerpAlpha);

		SetRelativeLocation(NewLocation);
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
