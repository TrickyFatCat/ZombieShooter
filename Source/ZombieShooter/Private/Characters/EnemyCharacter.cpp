// Created by Artyom Volkov during the UE4 course


#include "Characters/EnemyCharacter.h"

void AEnemyCharacter::OnDeath(AController* DeathInstigator, AActor* DeathCauser, const UDamageType* DamageType)
{
	Super::OnDeath(DeathInstigator, DeathCauser, DamageType);

	if (BaseExplosionClass && DamageType->IsA(BaseExplosionClass))
	{
		GetMesh()->SetSimulatePhysics(true);
		return;
	}

	const float DotProduct = FVector::DotProduct(GetActorForwardVector(),
	                                             DeathInstigator->GetPawn()->GetActorForwardVector());
	const FName StartSectionName = DotProduct > 0.f ? DeathAnimMontageSections[0] : DeathAnimMontageSections[1];
	PlayAnimMontage(DeathAnimMontage, 1.f, StartSectionName);
	SetLifeSpan(DefaultLifeSpan);
}
