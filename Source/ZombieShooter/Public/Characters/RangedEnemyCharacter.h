// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Characters/EnemyCharacter.h"
#include "RangedEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API ARangedEnemyCharacter : public AEnemyCharacter
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	virtual void StartAttack(const EEnemyMeleeAttackType AttackType) override;

	virtual void FinishAttack() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy|RangedAttack", meta=(AllowPrivateAccess="true"))
	TSubclassOf<AProjectileBase> SpitProjectile = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy|RangedAttack", meta=(AllowPrivateAccess="true"))
	int32 ProjectilesAmount = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy|RangedAttack", meta=(AllowPrivateAccess="true"))
	float RateOfFire = 1.f;

	float TimeBetweenAttacks = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy|RangedAttack", meta=(AllowPrivateAccess="true"))
	int32 MaxAttackNumber = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy|RangedAttack", meta=(AllowPrivateAccess="true"))
	float AttackSpread = 5.f;

	const float TraceDistance = 10000;

	int32 CurrentAttackNumber = 0;

	FTimerHandle AttackTimerHandle;

	UFUNCTION()
	void SpawnProjectiles();
};
