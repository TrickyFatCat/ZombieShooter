// Created by Artyom Volkov during the UE4 course


#include "AI/Tasks/BTTask_GetPointNearActor.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_GetPointNearActor::UBTTask_GetPointNearActor()
{
}

EBTNodeResult::Type UBTTask_GetPointNearActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	AActor* Owner = OwnerComp.GetOwner();

	if (!Blackboard || !Owner) return EBTNodeResult::Failed;

	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(Owner);
	if (!TargetActor || !NavSystem) return EBTNodeResult::Failed;

	FNavLocation TargetNavLocation;
	bool bLocationFound = false;
	const FRotator SearchCone = FRotator(0.f, FMath::FRandRange(-SearchConeHalfAngle, SearchConeHalfAngle), 0.f);

	do
	{
		FVector DirectionToOwner = SearchCone.RotateVector(Owner->GetActorLocation() - TargetActor->GetTargetLocation()).GetSafeNormal();
		const FVector Point = TargetActor->GetActorLocation() + DirectionToOwner * FMath::FRandRange(
			DistanceMin,
			DistanceMax);
		bLocationFound = NavSystem->GetRandomReachablePointInRadius(Point, SearchRadius, TargetNavLocation);
	}
	while (!bLocationFound);

	Blackboard->SetValueAsVector(TargetLocationKey.SelectedKeyName, TargetNavLocation);
	return EBTNodeResult::Succeeded;
}
