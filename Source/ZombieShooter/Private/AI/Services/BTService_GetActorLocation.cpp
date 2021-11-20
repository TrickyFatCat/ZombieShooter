// Created by Artyom Volkov during the UE4 course


#include "AI/Services/BTService_GetActorLocation.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTService_GetActorLocation::UBTService_GetActorLocation()
{
	NodeName = "Get Actor Location";
}

void UBTService_GetActorLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (Blackboard)
	{
		AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));

		if (TargetActor)
		{
			Blackboard->SetValueAsVector(TargetActorLocationKey.SelectedKeyName, TargetActor->GetActorLocation());
		}
	}
	
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
