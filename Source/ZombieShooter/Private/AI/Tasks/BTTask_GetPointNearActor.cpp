// Created by Artyom Volkov during the UE4 course


#include "AI/Tasks/BTTask_GetPointNearActor.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"

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

	do
	{
		const FRotator SearchCone = FRotator(0.f, FMath::FRandRange(-SearchConeHalfAngle, SearchConeHalfAngle), 0.f);
		const FVector DirectionToOwner = SearchCone.RotateVector(Owner->GetActorLocation() - TargetActor->GetTargetLocation())
		                                     .GetSafeNormal();
		const FVector Point = TargetActor->GetActorLocation() + DirectionToOwner * FMath::FRandRange(
			DistanceMin,
			DistanceMax);
		bLocationFound = NavSystem->GetRandomPointInNavigableRadius(Point, SearchRadius, TargetNavLocation);

		if (bLocationFound)
		{
			FHitResult HitResult;
			FCollisionQueryParams CollisionQueryParams;
			CollisionQueryParams.AddIgnoredActor(Owner);

			UKismetSystemLibrary::SphereTraceSingle(GetWorld(),
			                                        Point,
			                                        TargetActor->GetActorLocation(),
			                                        64,
			                                        UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),
			                                        false,
			                                        {Owner},
			                                        EDrawDebugTrace::ForDuration,
			                                        HitResult,
			                                        true, FLinearColor::Green, FLinearColor::Red, 0.1f);

			bLocationFound = !HitResult.bBlockingHit;

			if (bLocationFound) break;
		}
	}
	while (!bLocationFound);
	
	if (!bLocationFound) return EBTNodeResult::Failed;

	Blackboard->SetValueAsVector(TargetLocationKey.SelectedKeyName, TargetNavLocation);
	return EBTNodeResult::Succeeded;
}
