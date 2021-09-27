// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Actors/BaseEnemySpawn.h"
#include "EnemySpawnPoint.generated.h"

class UArrowComponent;

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API AEnemySpawnPoint : public ABaseEnemySpawn
{
	GENERATED_BODY()

public:
	AEnemySpawnPoint();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components")
	UArrowComponent* ForwardVectorPointer = nullptr;
	
	virtual FTransform CalculateSpawnTransform() override;
};
