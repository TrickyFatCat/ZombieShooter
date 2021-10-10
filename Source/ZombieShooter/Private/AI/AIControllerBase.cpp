// Created by Artyom Volkov during the UE4 course


#include "AI/AIControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"

AAIControllerBase::AAIControllerBase()
{
	Perception = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");
}

void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	Perception->OnPerceptionUpdated.AddDynamic(this, &AAIControllerBase::OnPerceptionUpdated);
}

EEnemyGeneralState AAIControllerBase::GetCurrentGeneralState() const
{
	return static_cast<EEnemyGeneralState>(Blackboard->GetValueAsEnum(GeneralStateKeyName));
}

void AAIControllerBase::SetGeneralState(const EEnemyGeneralState NewState) const
{
	if (GetCurrentGeneralState() == NewState) return;

	Blackboard->SetValueAsEnum(GeneralStateKeyName, static_cast<uint8>(NewState));
}

void AAIControllerBase::OnPerceptionUpdated(const TArray<AActor*>& Actors)
{
	FActorPerceptionBlueprintInfo PerceptionInfo;
	
	for (AActor* Actor : Actors)
	{
		Perception->GetActorsPerception(Actor, PerceptionInfo);
	}
}
