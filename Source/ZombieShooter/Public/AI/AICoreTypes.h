// Created by Artyom Volkov during the UE4 course

#pragma once

#include "AICoreTypes.generated.h"

UENUM(Blueprintable, BlueprintType)
enum class EEnemyGeneralState : uint8
{
	Idle,
	Roam,
	Patrol UMETA(Hidden),
	Investigate UMETA(Hidden),
	Stun UMETA(Hidden),
	Aggressive UMETA(Hidden)	
};
