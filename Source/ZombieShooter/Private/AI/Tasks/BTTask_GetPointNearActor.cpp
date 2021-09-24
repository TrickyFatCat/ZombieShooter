// Created by Artyom Volkov during the UE4 course


#include "AI/Tasks/BTTask_GetPointNearActor.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTTask_GetPointNearActor::UBTTask_GetPointNearActor()
{
}

EBTNodeResult::Type UBTTask_GetPointNearActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	AActor* Owner = OwnerComp.GetOwner();

	if (!Blackboard || !Owner) return EBTNodeResult::Failed;

	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName)); 

	if (!TargetActor) return EBTNodeResult::Failed;

	const FVector DirectionToOwner = (Owner->GetActorLocation() - TargetActor->GetTargetLocation()).GetSafeNormal();
	const FVector Point = TargetActor->GetActorLocation() + DirectionToOwner * FMath::FRandRange(DistanceMin, DistanceMax);
	DrawDebugSphere(Owner->GetWorld(), Point, SearchRadius, 5, FColor::Emerald, false, -1, 0, 2);

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(Owner);

	if (!NavSystem) return EBTNodeResult::Failed;

	FNavLocation TargetNavLocation;
	const bool bLocationFound = NavSystem->GetRandomReachablePointInRadius(Point, SearchRadius, TargetNavLocation);

	if (!bLocationFound) return EBTNodeResult::Failed;

	Blackboard->SetValueAsVector(TargetLocationKey.SelectedKeyName, TargetNavLocation);
	return EBTNodeResult::Succeeded;
}
