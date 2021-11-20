// Created by Artyom Volkov during the UE4 course


#include "AI/Decorators/BTDecorator_IsTargetAlive.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Core/ProjectUtils.h"

UBTDecorator_IsTargetAlive::UBTDecorator_IsTargetAlive()
{
	NodeName = "Is Target Alive";
}

bool UBTDecorator_IsTargetAlive::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (!Blackboard) return false;

	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));

	if (!TargetActor) return false;

	return !FProjectUtils::GetIsActorDead(TargetActor);
}
