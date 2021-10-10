// Created by Artyom Volkov during the UE4 course

#pragma once

#include "AICoreTypes.generated.h"

UENUM(Blueprintable, BlueprintType)
enum class EEnemyGeneralState : uint8
{
	Idle,
	Roaming,
	Patrolling,
	Investigating,
	Stunned,
	Attacking
};
