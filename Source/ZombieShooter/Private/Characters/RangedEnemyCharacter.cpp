// Created by Artyom Volkov during the UE4 course


#include "Characters/RangedEnemyCharacter.h"

#include "AIController.h"
#include "Weapons/ProjectileBase.h"

void ARangedEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (RateOfFire <= 0.f)
	{
		RateOfFire = 1.f;
	}
	
	TimeBetweenAttacks = 1.f / RateOfFire;
}

void ARangedEnemyCharacter::StartAttack(const EEnemyAttackType Attack)
{
	if (Attack != EEnemyAttackType::Ranged)
	{
		Super::StartAttack(Attack);
		return;
	}
	
	if (!GetWorld() || !SpitProjectile) return;

	SpawnProjectiles();

	if (CurrentAttackNumber < MaxAttackNumber)
	{
		GetWorldTimerManager().SetTimer(AttackTimerHandle,
		                                this,
		                                &ARangedEnemyCharacter::SpawnProjectiles,
		                                TimeBetweenAttacks,
		                                true);
	}
}

void ARangedEnemyCharacter::FinishAttack()
{
	if (CurrentAttackType != EEnemyAttackType::Ranged)
	{
		Super::FinishAttack();
		return;
	}
	
	CurrentAttackNumber = 0;
	
	if (GetWorldTimerManager().IsTimerActive(AttackTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(AttackTimerHandle);
	}
}

void ARangedEnemyCharacter::SpawnProjectiles()
{
	if (!GetWorld() || !SpitProjectile) return;

	FVector TraceStart, TraceEnd;
	FRotator ViewRotation;
	FHitResult HitResult;

	AAIController* AIController = Cast<AAIController>(GetController());

	if (!Controller)
	{
		FinishAttack();
		return;
	}

	Controller->GetActorEyesViewPoint(TraceStart, ViewRotation);
	const FVector MouthSocketLocation = GetMesh()->GetSocketLocation(MouthSocketName);
	FVector AttackDirection = ViewRotation.Vector();
	int32 ProjectileDamage = FMath::Max(AttackDamage / ProjectilesAmount, 1);

	for (int32 i = 1; i <= ProjectilesAmount; ++i)
	{
		if (AttackSpread > 0.f)
		{
			const float SpreadAngleRad = FMath::DegreesToRadians(AttackSpread / 2);
			AttackDirection = FMath::VRandCone(AttackDirection, SpreadAngleRad);
		}

		TraceEnd = TraceStart + AttackDirection * TraceDistance;

		FCollisionObjectQueryParams CollisionObjectQueryParams = FCollisionObjectQueryParams(
			ECollisionChannel::ECC_GameTraceChannel2);
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(this);

		GetWorld()->LineTraceSingleByObjectType(HitResult,
		                                        TraceStart,
		                                        TraceEnd,
		                                        CollisionObjectQueryParams,
		                                        CollisionQueryParams);

		TraceEnd = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
		FVector Direction = (TraceEnd - TraceStart).GetSafeNormal();

		if (!SpitProjectile) break;

		const FTransform SpawnTransform(FRotator::ZeroRotator, MouthSocketLocation);
		AProjectileBase* Projectile = GetWorld()->SpawnActorDeferred<AProjectileBase>(
			SpitProjectile,
			SpawnTransform);

		if (Projectile)
		{
			Projectile->SetDirectionAndDamage(Direction, nullptr, ProjectileDamage);
			Projectile->SetOwner(GetOwner());
			Projectile->FinishSpawning(SpawnTransform);
		}
	}

	if (++CurrentAttackNumber >= MaxAttackNumber)
	{
		FinishAttack();
	}
}
