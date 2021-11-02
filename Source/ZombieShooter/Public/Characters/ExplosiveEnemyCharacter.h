// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Characters/RangedEnemyCharacter.h"
#include "ExplosiveEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API AExplosiveEnemyCharacter : public AEnemyCharacter
{
	GENERATED_BODY()

public:
	AExplosiveEnemyCharacter();

protected:
	virtual void OnDeath(AController* DeathInstigator, AActor* DeathCauser, const UDamageType* DamageType) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy|Attack|Explosion")
	bool bDealFullDamage = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy|Attack|Explosion")
	float ExplosionDamage = 30.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy|Attack|Explosion")
	float ExplosionRadius = 200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy|Attack|Explosion")
	TSubclassOf<UDamageType> ExplosionDamageType = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy|Attack|Explosion")
	UParticleSystem* ExplosionEffect = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy|Attack|Explosion")
	USoundCue* ExplosionSound = nullptr;
};
