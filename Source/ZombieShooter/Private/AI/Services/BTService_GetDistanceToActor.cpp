// Created by Artyom Volkov during the UE4 course


#include "AI/Services/BTService_GetDistanceToActor.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTService_GetDistanceToActor::UBTService_GetDistanceToActor()
{
	NodeName = "Get Distance To Actor";
}

void UBTService_GetDistanceToActor::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	AActor* Owner = OwnerComp.GetOwner();

	if (Blackboard && Owner)
	{
		AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));

		if (TargetActor)
		{
			const float Distance = (Owner->GetActorLocation() - TargetActor->GetActorLocation()).Size();
			Blackboard->SetValueAsFloat(DistanceKey.SelectedKeyName, Distance);
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
