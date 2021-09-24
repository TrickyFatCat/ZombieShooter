// Created by Artyom Volkov during the UE4 course


#include "Characters/Controllers/ZombieAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/EnemyCharacter.h"
#include "AI/ZombiePerceptionComponent.h"


AZombieAIController::AZombieAIController()
{
	Perception = CreateDefaultSubobject<UZombiePerceptionComponent>("PerceptionComponent");
}

void AZombieAIController::BeginPlay()
{
	Super::BeginPlay();

	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	
	if (BlackboardComponent)
	{
		BlackboardComponent->SetValueAsVector(InitialLocationKeyName, GetPawn()->GetActorLocation());

		AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetPawn());

		if (EnemyCharacter)
		{
			const bool IsPatrolling = EnemyCharacter->GetInitialBehavior() == EEnemyInitialBehavior::Patrol;
			BlackboardComponent->SetValueAsBool(IsPatrollingKeyName, IsPatrolling);
		}
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

	if (GetBlackboardComponent())
	{
		GetBlackboardComponent()->SetValueAsBool(IsAttackingKeyName, false);
	}
}

AActor* AZombieAIController::GetTargetActor() const
{
	if (!GetBlackboardComponent()) return nullptr;

	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(TargetActorKeyName));
}
