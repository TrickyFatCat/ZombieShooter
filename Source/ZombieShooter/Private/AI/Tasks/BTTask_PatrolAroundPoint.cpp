// Created by Artyom Volkov during the UE4 course


#include "AI/Tasks/BTTask_PatrolAroundPoint.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_PatrolAroundPoint::UBTTask_PatrolAroundPoint()
{
	NodeName="Patrol around a point";
}

EBTNodeResult::Type UBTTask_PatrolAroundPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	if (!AIController || !BlackboardComponent) return EBTNodeResult::Failed;

	APawn* Pawn = AIController->GetPawn();

	if (!Pawn) return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(Pawn);

	if (!NavSystem) return EBTNodeResult::Failed;

	FNavLocation TargetLocation;
	const bool bLocationFound = NavSystem->GetRandomReachablePointInRadius(
		BlackboardComponent->GetValueAsVector(InitialLocationKey.SelectedKeyName),
		Radius,
		TargetLocation);

	if (!bLocationFound) return EBTNodeResult::Failed;

	BlackboardComponent->SetValueAsVector(TargetLocationKey.SelectedKeyName, TargetLocation);
	return EBTNodeResult::Succeeded;
}
