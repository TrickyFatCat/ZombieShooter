// Created by Artyom Volkov during the UE4 course


#include "AI/Services/BTService_CheckLineOfSight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

UBTService_CheckLineOfSight::UBTService_CheckLineOfSight()
{
	NodeName = "Is In Line Of Sight";
}

void UBTService_CheckLineOfSight::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	bool bIsInLineOfSight = false;
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	AActor* Owner = OwnerComp.GetOwner();
	UWorld* World = GetWorld();

	if (Blackboard && Owner && World)
	{
		AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));

		if (TargetActor)
		{
			FHitResult HitResult;
			FCollisionQueryParams CollisionQueryParams;
			CollisionQueryParams.AddIgnoredActor(Owner);

			UKismetSystemLibrary::SphereTraceSingle(World,
			                                        Owner->GetActorLocation(),
			                                        TargetActor->GetActorLocation(),
			                                        64,
			                                        UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),
			                                        false,
			                                        {Owner},
			                                        EDrawDebugTrace::None,
			                                        HitResult,
			                                        true);
			bIsInLineOfSight = !HitResult.bBlockingHit;
		}
	}
	Blackboard->SetValueAsBool(IsInLineOfSightKey.SelectedKeyName, bIsInLineOfSight);
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
