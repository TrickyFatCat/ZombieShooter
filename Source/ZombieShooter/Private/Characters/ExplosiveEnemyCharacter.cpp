// Created by Artyom Volkov during the UE4 course


#include "Characters/ExplosiveEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/Components/WeaponFXComponent.h"

AExplosiveEnemyCharacter::AExplosiveEnemyCharacter()
{
	ExplosionFXComponent = CreateDefaultSubobject<UWeaponFXComponent>("ExplosionFXComponent");
}

void AExplosiveEnemyCharacter::OnDeath(AController* DeathInstigator, AActor* DeathCauser, const UDamageType* Damage)
{
	UGameplayStatics::ApplyRadialDamage(GetWorld(),
	                                    ExplosionDamage,
	                                    GetActorLocation(),
	                                    ExplosionRadius,
	                                    ExplosionDamageType,
	                                    {},
	                                    this,
	                                    GetController(),
	                                    bDealFullDamage,
	                                    ECC_GameTraceChannel1);

	FHitResult HitResult;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(HitResult,
	                                     GetActorLocation(),
	                                     GetActorLocation() + GetActorUpVector() * -100,
	                                     ECC_Visibility,
	                                     CollisionQueryParams);

	ExplosionFXComponent->PlayImpactFX(HitResult);
	Super::OnDeath(DeathInstigator, DeathCauser, Damage);
}
