// Created by Artyom Volkov during the UE4 course

#pragma once

#include "EnemyCoreTypes.generated.h"

USTRUCT(BlueprintType)
struct FEnemyMovementData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enemy", meta=(ClampMin="0"))
	float WalkingSpeedDefault = 200.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enemy", meta=(ClampMin="0"))
	float WalkingSpeedRandomDeviation = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enemy", meta=(ClampMin="0"))
	float RunningSpedDefault = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enemy", meta=(ClampMin="0"))
	float RunningSpeedRandomDeviation = 0.f;
};

UENUM(BlueprintType)
enum class EEnemyMeleeAttackType : uint8
{
	Right,
	Left,
	Bite
};