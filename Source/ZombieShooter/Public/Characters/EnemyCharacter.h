// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/EnemyCoreTypes.h"
#include "EnemyCharacter.generated.h"

class UAnimMontage;
class UBehaviorTree;

UENUM(BlueprintType)
enum class EEnemyInitialBehavior : uint8
{
	Idle,
	Patrol
};

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
	virtual void BeginPlay() override;

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

	EEnemyInitialBehavior GetInitialBehavior() const { return InitialBehavior; }

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy|AI", meta=(AllowPrivateAccess="true"))
	UBehaviorTree* BehaviorTree = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enemy|AI", meta=(AllowPrivateAccess="true"))
	EEnemyInitialBehavior InitialBehavior = EEnemyInitialBehavior::Idle;

	// Movement
public:
	UFUNCTION(BlueprintGetter, Category="Enemy")
	FEnemyMovementData GetMovementData() const { return MovementData; }
	
	UFUNCTION(BlueprintPure, Category="Enemy")
	bool IsRunning() const;

	void SetIsRunning(const bool bIsRunning) const;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintGetter=GetMovementData, Category="Enemy", meta=(AllowPrivateAccess="true"))
	FEnemyMovementData MovementData;
};
