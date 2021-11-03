// Created by Artyom Volkov during the UE4 course


#include "AI/Services/BTService_GetPointNearActor.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UBTService_GetPointNearActor::UBTService_GetPointNearActor()
{
	NodeName = "Find Point Near Actor";
}

void UBTService_GetPointNearActor::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	AActor* Owner = OwnerComp.GetOwner();

	if (Blackboard && Owner)
	{
		FNavLocation TargetNavLocation;
		bool bLocationFound = false;
		FVector Point = FVector::ZeroVector;
		AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(Owner);
		if (TargetActor && NavSystem)
		{
			const FRotator SearchCone = FRotator(0.f,
			                                     FMath::FRandRange(-SearchConeHalfAngle, SearchConeHalfAngle),
			                                     0.f);
			const FVector DirectionToOwner = SearchCone.RotateVector(
				                                           Owner->GetActorLocation() - TargetActor->
				                                           GetTargetLocation())
			                                           .GetSafeNormal();
			Point = TargetActor->GetActorLocation() + DirectionToOwner * FMath::FRandRange(
				DistanceMin,
				DistanceMax);
			bLocationFound = NavSystem->GetRandomPointInNavigableRadius(
				Point,
				SearchRadius,
				TargetNavLocation);
		}

		if (bCheckLineOfSight && bLocationFound)
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
			                                        true,
			                                        FLinearColor::Green,
			                                        FLinearColor::Red,
			                                        0.1f);

			bLocationFound = !HitResult.bBlockingHit;

			if (bLocationFound)
			{
				Blackboard->SetValueAsVector(TargetLocationKey.SelectedKeyName, TargetNavLocation);
			}
		}
	}


	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
