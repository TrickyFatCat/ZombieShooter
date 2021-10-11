// Created by Artyom Volkov during the UE4 course


#include "AI/AIControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Characters/EnemyCharacter.h"
#include "Characters/PlayerCharacter.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Hearing.h"
#include "Core/ProjectUtils.h"

AAIControllerBase::AAIControllerBase()
{
	Perception = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");
	bAttachToPawn = true;
}

void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Perception->OnPerceptionUpdated.AddDynamic(this, &AAIControllerBase::OnPerceptionUpdated);

	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetPawn());

	if (EnemyCharacter)
	{
		RunBehaviorTree(EnemyCharacter->GetBehaviorTree());
		SetGeneralState(EnemyCharacter->GetInitialState());
		Blackboard->SetValueAsVector(SpawnLocationKeyName, EnemyCharacter->GetActorLocation());
	}
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

void AAIControllerBase::SetTargetActor(AActor* TargetActor) const
{
	Blackboard->SetValueAsObject(TargetActorKeyName, TargetActor);
}

void AAIControllerBase::OnPerceptionUpdated(const TArray<AActor*>& Actors)
{
	FActorPerceptionBlueprintInfo PerceptionInfo;
	AActor* SensedActor = nullptr;

	for (AActor* Actor : Actors)
	{
		Perception->GetActorsPerception(Actor, PerceptionInfo);

		SensedActor = PerceptionInfo.Target;

		if (!SensedActor->IsA(APlayerCharacter::StaticClass())) break;

		if (FProjectUtils::GetIsActorDead(SensedActor)) break;

		for (FAIStimulus& Stimuli : PerceptionInfo.LastSensedStimuli)
		{
			const TSubclassOf<UAISense> SenseClass = UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimuli);

			if (SenseClass == UAISense_Sight::StaticClass())
			{
				SetTargetActor(SensedActor);
				SetGeneralState(EEnemyGeneralState::Aggressive);
				break;
			}

			if (SenseClass == UAISense_Damage::StaticClass())
			{
				SetGeneralState(EEnemyGeneralState::Investigate);
				break;
			}

			if (SenseClass == UAISense_Hearing::StaticClass())
			{
				SetGeneralState(EEnemyGeneralState::Investigate);
			}
		}
	}
}
