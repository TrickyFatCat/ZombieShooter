// Created by Artyom Volkov during the UE4 course


#include "AI/ZombiePerceptionComponent.h"
#include "AIController.h"
#include "Characters/PlayerCharacter.h"
#include "Components/ShooterDamageControllerComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Damage.h"

AActor* UZombiePerceptionComponent::GetPlayer() const
{
	TArray<AActor*> PerceivedActors;
	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);

	if (PerceivedActors.Num() == 0)
	{
		GetCurrentlyPerceivedActors(UAISense_Damage::StaticClass(), PerceivedActors);

		if (PerceivedActors.Num() == 0) return nullptr;
	}

	AAIController* AIController = Cast<AAIController>(GetOwner());

	if (!AIController) return nullptr;

	APawn* Pawn = AIController->GetPawn();

	if (!Pawn) return nullptr;

	AActor* Player = nullptr;

	for (AActor* PerceivedActor : PerceivedActors)
	{
		if (!PerceivedActor->IsA(APlayerCharacter::StaticClass())) continue;

		UShooterDamageControllerComponent* DamageController = PerceivedActor->FindComponentByClass<UShooterDamageControllerComponent>();

		if (DamageController && !DamageController->GetIsDead())
		{
			Player = PerceivedActor;
			break;
		}
	}
	return Player;
}
