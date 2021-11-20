// Created by Artyom Volkov during the UE4 course


#include "AI/EnemyFunctionLibrary.h"
#include "Characters/EnemyCharacter.h"

void UEnemyFunctionLibrary::AggroEnemies(TArray<AEnemyCharacter*> Enemies)
{
	for (const auto& Enemy : Enemies)
	{
		if (!IsValid(Enemy)) continue;

		Enemy->AttackPlayer();
	}
}

void UEnemyFunctionLibrary::AggroEnemy(AEnemyCharacter* Enemy)
{
	if (!IsValid(Enemy)) return;

	Enemy->AttackPlayer();
}
