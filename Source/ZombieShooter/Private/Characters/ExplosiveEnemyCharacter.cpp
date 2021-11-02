// Created by Artyom Volkov during the UE4 course


#include "Characters/ExplosiveEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

AExplosiveEnemyCharacter::AExplosiveEnemyCharacter()
{
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
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
	Super::OnDeath(DeathInstigator, DeathCauser, Damage);
}
