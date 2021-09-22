// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "EnemyCharacter.generated.h"

class UAnimMontage;

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

	// Death
protected:
	virtual void OnDeath(AController* DeathInstigator, AActor* DeathCauser, const UDamageType* DamageType) override;

private:
	UPROPERTY(EditDefaultsOnly, Category="Enemy|Animations")
	UAnimMontage* DeathAnimMontage = nullptr;

	UPROPERTY(VisibleDefaultsOnly, Category="Enemy|Animations")
	TArray<FName> DeathAnimMontageSections = {FName("Death_Front"), FName("Death_Back")};

	UPROPERTY(EditDefaultsOnly, Category="Enemy|Animations")
	TSubclassOf<UDamageType> BaseExplosionClass = nullptr;

	const float DefaultLifeSpan = 5.f;
};
