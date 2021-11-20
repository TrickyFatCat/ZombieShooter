// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Actors/EnemySpawns/BaseEnemySpawn.h"
#include "EnemySpawnArea.generated.h"

class UBoxComponent;
/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API AEnemySpawnArea : public ABaseEnemySpawn
{
	GENERATED_BODY()

public:
	AEnemySpawnArea();
protected:
	virtual FTransform CalculateSpawnTransform() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Compnonents")
	UBoxComponent* SpawnArea = nullptr;
};
