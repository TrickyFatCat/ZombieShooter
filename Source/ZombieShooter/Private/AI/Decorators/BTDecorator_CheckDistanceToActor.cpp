// Created by Artyom Volkov during the UE4 course


#include "AI/Decorators/BTDecorator_CheckDistanceToActor.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_CheckDistanceToActor::UBTDecorator_CheckDistanceToActor()
{
	NodeName = "Check Distance To Target Actor";
}

bool UBTDecorator_CheckDistanceToActor::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
                                                                   uint8* NodeMemory) const
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (!Blackboard) return false;

	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));

	if (!TargetActor) return false;

	const float CurrentDistance = Blackboard->GetValueAsFloat(DistanceToActorKey.SelectedKeyName);
	const bool Result = CheckMethod == EDistanceCheckMethod::IsFar
		                    ? CurrentDistance > TargetDistance
		                    : CurrentDistance <= TargetDistance;

	return Result;
}
