// Created by Artyom Volkov during the UE4 course


#include "AI/Decorators/BTDecorator_CheckIsStunned.h"

#include "Characters/EnemyCharacter.h"
#include "AIController.h"

UBTDecorator_CheckIsStunned::UBTDecorator_CheckIsStunned()
{
	NodeName = "Check Is Stunned";
}

bool UBTDecorator_CheckIsStunned::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (!EnemyCharacter) return false;

	return EnemyCharacter->GetIsStunned();
}
