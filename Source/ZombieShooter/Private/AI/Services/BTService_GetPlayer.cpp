// Created by Artyom Volkov during the UE4 course


#include "AI/Services/BTService_GetPlayer.h"

#include "AIController.h"
#include "AI/ZombiePerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_GetPlayer::UBTService_GetPlayer()
{
	NodeName = "Get player";
}

void UBTService_GetPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (Blackboard)
	{
		const AAIController* AIController = OwnerComp.GetAIOwner();
		UZombiePerceptionComponent* PerceptionComponent = AIController->FindComponentByClass<UZombiePerceptionComponent>();

		if (PerceptionComponent)
		{
			AActor* Player = PerceptionComponent->GetPlayer();
			Blackboard->SetValueAsObject(TargetActorKey.SelectedKeyName, Player);

			if (Player)
			{
				Blackboard->SetValueAsVector(TargetActorLocationKey.SelectedKeyName, Player->GetActorLocation());
			}
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
