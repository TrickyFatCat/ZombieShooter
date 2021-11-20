// Created by Artyom Volkov during the UE4 course


#include "AI/Tasks/BTTask_SetBoolValue.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetBoolValue::UBTTask_SetBoolValue()
{
	NodeName = "Set Bool Value";
}

EBTNodeResult::Type UBTTask_SetBoolValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (!Blackboard) return EBTNodeResult::Failed;

	if (Blackboard->GetValueAsBool(BoolValueKey.SelectedKeyName) == bNewValue) return EBTNodeResult::Failed;
	
	Blackboard->SetValueAsBool(BoolValueKey.SelectedKeyName, bNewValue);

	return EBTNodeResult::Succeeded;
}
