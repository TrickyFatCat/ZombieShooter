// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "ZombiePerceptionComponent.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API UZombiePerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:
	AActor* GetPlayer() const;
};
