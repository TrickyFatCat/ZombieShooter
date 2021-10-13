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
#include "Kismet/GameplayStatics.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "DrawDebugHelpers.h"
#include "Debug/DebugDrawService.h"
#include "TraceServices/Public/TraceServices/Containers/Timelines.h"

AAIControllerBase::AAIControllerBase(const FObjectInitializer& ObjectInitializer): Super(
	ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
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

void AAIControllerBase::SetInvestigationLocation(const FVector Location) const
{
	Blackboard->SetValueAsVector(InvestigationLocationKeyName, Location);
}


void AAIControllerBase::StartInvestigation(const FVector Location) const
{
	if (GetCurrentGeneralState() == EEnemyGeneralState::Investigate) return;

	StartRunning();
	SetIsScreaming(true);
	SetInvestigationLocation(Location);
	SetGeneralState(EEnemyGeneralState::Investigate);
}

void AAIControllerBase::StartAttacking(AActor* TargetActor) const
{
	if (GetCurrentGeneralState() == EEnemyGeneralState::Aggressive) return;

	SetTargetActor(TargetActor);
	StartRunning();
	SetIsScreaming(true);
	SetGeneralState(EEnemyGeneralState::Aggressive);
}

void AAIControllerBase::SetIsScreaming(const bool bIsScreaming) const
{
	Blackboard->SetValueAsBool(IsScreamingKeyName, bIsScreaming);
}

void AAIControllerBase::StartRunning() const
{
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetPawn());

	if (EnemyCharacter)
	{
		EnemyCharacter->SetIsRunning(true);
	}
}

void AAIControllerBase::OnPerceptionUpdated(const TArray<AActor*>& Actors)
{
	FActorPerceptionBlueprintInfo PerceptionInfo;
	AActor* SensedActor = nullptr;

	for (AActor* Actor : Actors)
	{
		Perception->GetActorsPerception(Actor, PerceptionInfo);

		SensedActor = PerceptionInfo.Target;


		if (FProjectUtils::GetIsActorDead(SensedActor)) break;

		for (FAIStimulus& Stimuli : PerceptionInfo.LastSensedStimuli)
		{
			const TSubclassOf<UAISense> SenseClass = UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimuli);

			if (SenseClass == UAISense_Sight::StaticClass())
			{
				if (!SensedActor->IsA(APlayerCharacter::StaticClass())) continue;

				StartAttacking(SensedActor);
				break;
			}
			else if (SenseClass == UAISense_Hearing::StaticClass())
			{
				if (GetCurrentGeneralState() == EEnemyGeneralState::Aggressive) continue;
				FHitResult OutHit;
				FCollisionQueryParams QueryParams;
				AActor* TargetActor = SensedActor;
				QueryParams.AddIgnoredActor(this);
				QueryParams.AddIgnoredActor(GetPawn());
				QueryParams.AddIgnoredActor(SensedActor);
				GetWorld()->LineTraceSingleByChannel(OutHit,
				                                     GetPawn()->GetActorLocation(),
				                                     Stimuli.StimulusLocation,
				                                     ECollisionChannel::ECC_GameTraceChannel3,
				                                     QueryParams);

				if (OutHit.bBlockingHit && !Stimuli.StimulusLocation.Equals(OutHit.ImpactPoint, 0.01f))
				{
					Stimuli.MarkExpired();
					continue;
				}
				
				if (SensedActor->IsA(AEnemyCharacter::StaticClass()))
				{
					AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(SensedActor);

					if (!EnemyCharacter) continue;

					TargetActor = EnemyCharacter->GetTargetActor();
				}
				
				StartAttacking(TargetActor);
				// StartInvestigation(Stimuli.StimulusLocation);
			}
			else if (SenseClass == UAISense_Damage::StaticClass())
			{
				if (!SensedActor->IsA(APlayerCharacter::StaticClass())) continue;

				StartAttacking(SensedActor);
				// StartInvestigation(Stimuli.ReceiverLocation);
				break;
			}
		}
	}
}

AActor* AAIControllerBase::GetTargetActor() const
{
	return Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKeyName));
}
