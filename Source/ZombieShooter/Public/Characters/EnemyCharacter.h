// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "EnemyCharacter.generated.h"

class UAnimMontage;
class UBehaviorTree;

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

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

	// Behaviour tree
	public:
	UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }

	private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy|AI", meta=(AllowPrivateAccess))
	UBehaviorTree* BehaviorTree = nullptr;
	
};
