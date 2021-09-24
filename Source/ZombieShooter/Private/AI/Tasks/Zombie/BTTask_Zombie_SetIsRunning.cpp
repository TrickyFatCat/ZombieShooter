// Created by Artyom Volkov during the UE4 course


#include "AI/Tasks/Zombie/BTTask_Zombie_SetIsRunning.h"

#include "AIController.h"
#include "Characters/EnemyCharacter.h"

UBTTask_Zombie_SetIsRunning::UBTTask_Zombie_SetIsRunning()
{
	NodeName="Set Is Running";
}

EBTNodeResult::Type UBTTask_Zombie_SetIsRunning::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (!EnemyCharacter) return EBTNodeResult::Failed;

	EnemyCharacter->SetIsRunning(bIsRunning);

	return EBTNodeResult::Succeeded;
}
