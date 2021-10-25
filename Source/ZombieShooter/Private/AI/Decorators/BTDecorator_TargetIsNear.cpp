// Created by Artyom Volkov during the UE4 course


#include "AI/Decorators/BTDecorator_TargetIsNear.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_TargetIsNear::UBTDecorator_TargetIsNear()
{
	NodeName = "Target Is Near";
}

bool UBTDecorator_TargetIsNear::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
                                                                   uint8* NodeMemory) const
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (!Blackboard) return false;

	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));

	if (!TargetActor) return false;

	const float CurrentDistance = Blackboard->GetValueAsFloat(DistanceToActorKey.SelectedKeyName);
	const bool Result = CurrentDistance <= TargetDistance;

	return Result;
}
