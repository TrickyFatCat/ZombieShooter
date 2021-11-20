// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseEnemySpawn.generated.h"

class AEnemyCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemySpawnedSignature, AEnemyCharacter*, SpawnedCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpawnStartedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpawnFinishedSignature);

USTRUCT(BlueprintType)
struct FSpawnData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawn")
	TSubclassOf<AEnemyCharacter> EnemyClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawn", meta=(ClampMin="0"))
	int32 Number = 1;
};

UCLASS()
class ZOMBIESHOOTER_API ABaseEnemySpawn : public AActor
{
	GENERATED_BODY()

public:
	ABaseEnemySpawn();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(BlueprintAssignable, Category="Spawn")
	FOnEnemySpawnedSignature OnEnemySpawned;

	UPROPERTY(BlueprintAssignable, Category="Spawn")
	FOnSpawnStartedSignature OnSpawnStarted;
	
	UPROPERTY(BlueprintAssignable, Category="Spawn")
	FOnSpawnFinishedSignature OnSpawnFinished;
	
	UFUNCTION(BlueprintCallable, Category="Spawn")
	void StartSpawn();

	UFUNCTION(BlueprintCallable, Category="Spawn")
	void StopSpawn();

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Spawn")
	TArray<FSpawnData> SpawnTable;

	UPROPERTY(VisibleAnywhere, Category="Spawn")
	TArray<FSpawnData> CachedSpawnTable;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Spawn", meta=(ClampMin="0"))
	float SpawnPause = 1.f;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Spawn", meta=(ClampMin="1"))
	int32 SpawnNumber = 1;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Spawn")
	bool bAggroEnemyOnSpawn = false;
	
	int32 CurrentEnemyID = 0;

	int32 SpawnedEnemiesCount = 0;

	int32 SpawnedEnemiesMax = 0;

	FTimerHandle SpawnTimerHandle;

	AEnemyCharacter* SpawnEnemy(const TSubclassOf<AEnemyCharacter> EnemyClass,
	                            const FTransform& SpawnTransform);

	UFUNCTION()
	void ProcessSpawnQueue();

	virtual FTransform CalculateSpawnTransform();

	bool IsEnemyIdOutOfBounds();
};
