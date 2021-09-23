// Created by Artyom Volkov during the UE4 course


#include "Characters/Controllers/ZombieAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/EnemyCharacter.h"


AZombieAIController::AZombieAIController()
{
}

void AZombieAIController::BeginPlay()
{
	Super::BeginPlay();


	if (GetBlackboardComponent())
	{
		GetBlackboardComponent()->SetValueAsVector(InitialLocationKeyName, GetPawn()->GetActorLocation());
	}
}

void AZombieAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AZombieAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AEnemyCharacter* AICharacter = Cast<AEnemyCharacter>(InPawn);

	if (AICharacter)
	{
		RunBehaviorTree(AICharacter->GetBehaviorTree());
	}
	

}

AActor* AZombieAIController::GetTargetActor() const
{
	if (!GetBlackboardComponent()) return nullptr;

	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(TargetActorKeyName));
}
