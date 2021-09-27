// Created by Artyom Volkov during the UE4 course


#include "Actors/EnemySpawns/BaseEnemySpawn.h"

#include "Characters/EnemyCharacter.h"
#include "Math/TransformCalculus3D.h"

ABaseEnemySpawn::ABaseEnemySpawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseEnemySpawn::BeginPlay()
{
	Super::BeginPlay();

	CachedSpawnTable = SpawnTable;

	for (const auto SpawnData : CachedSpawnTable)
	{
		if (!SpawnData.EnemyClass) continue;

		SpawnedEnemiesMax += SpawnData.Number;
	}
}

void ABaseEnemySpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseEnemySpawn::StartSpawn()
{
	if (SpawnTable.Num() <= 0 || SpawnedEnemiesMax <= 0) return;

	ProcessSpawnQueue();
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ABaseEnemySpawn::ProcessSpawnQueue, SpawnPause, true);
	OnSpawnStarted.Broadcast();
}

void ABaseEnemySpawn::StopSpawn()
{
	if (SpawnTable.Num() <= 0 || !GetWorldTimerManager().IsTimerActive(SpawnTimerHandle)) return;

	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
}

AEnemyCharacter* ABaseEnemySpawn::SpawnEnemy(const TSubclassOf<AEnemyCharacter> EnemyClass,
                                             const FTransform& SpawnTransform)
{
	AEnemyCharacter* EnemyCharacter = GetWorld()->SpawnActor<AEnemyCharacter>(EnemyClass, SpawnTransform);

	if (!EnemyCharacter) return nullptr;

	OnEnemySpawned.Broadcast(EnemyCharacter);

	return EnemyCharacter;
}

void ABaseEnemySpawn::ProcessSpawnQueue()
{
	if (CurrentEnemyID >= CachedSpawnTable.Num())
	{
		StopSpawn();
		return;
	}

	for (int32 i = 1; i <= SpawnNumber; ++i)
	{
		if (CachedSpawnTable[CurrentEnemyID].Number <= 0 || CachedSpawnTable[CurrentEnemyID].EnemyClass == nullptr)
		{
			CurrentEnemyID++;

			if (IsEnemyIdOutOfBounds()) break;

			continue;
		}

		AEnemyCharacter* SpawnedEnemy = SpawnEnemy(SpawnTable[CurrentEnemyID].EnemyClass, CalculateSpawnTransform());

		if (!SpawnedEnemy)
		{
			CurrentEnemyID++;

			if (IsEnemyIdOutOfBounds()) break;

			continue;
		}

		CachedSpawnTable[CurrentEnemyID].Number--;
		SpawnedEnemiesCount++;

		if (CachedSpawnTable[CurrentEnemyID].Number <= 0) CurrentEnemyID++;

		if (IsEnemyIdOutOfBounds()) break;
	}
}

FTransform ABaseEnemySpawn::CalculateSpawnTransform()
{
	return FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::OneVector);
}

bool ABaseEnemySpawn::IsEnemyIdOutOfBounds()
{
	if (CurrentEnemyID >= CachedSpawnTable.Num())
	{
		StopSpawn();
		OnSpawnFinished.Broadcast();
		return true;
	}

	return false;
}
