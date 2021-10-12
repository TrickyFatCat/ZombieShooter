// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/EnemyCoreTypes.h"
#include "AI/AICoreTypes.h"
#include "EnemyCharacter.generated.h"

class UAnimMontage;
class UBehaviorTree;
class UDamageSphereComponent;

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

	// Commands
public:
	UFUNCTION(BlueprintCallable, Category="Enemy")
	void AttackPlayer() const;

	// Behaviour tree
public:
	UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }

	EEnemyGeneralState GetInitialState() const { return InitialState; }

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy|AI", meta=(AllowPrivateAccess="true"))
	UBehaviorTree* BehaviorTree = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enemy|AI", meta=(AllowPrivateAccess="true"))
	EEnemyGeneralState InitialState;

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

	// Stun
public:
	bool GetIsStunned() const { return bIsStunned; };
	void SetIsStunned(const bool Value);
protected:
	UPROPERTY(VisibleAnywhere)
	bool bIsStunned = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy|HitReaction")
	UAnimMontage* HitReactionMontage = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy|HitReaction")
	TMap<TSubclassOf<UDamageType>, float> StunChancesMap;
	
	UFUNCTION()
	virtual void OnTakeDamage(AActor* DamageActor,
	                             float Damage,
	                             const UDamageType* DamageType,
	                             AController* InstigatedBy,
	                             AActor* DamageCauser);

	// Scream
public:
	void AggroNeighbours();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy", meta=(AllowPrivateAccess="true", ClampMin="0"))
	float ScreamRadius = 300.f;

	// Attack
public:
	virtual void StartAttack();
	
	virtual void FinishAttack();

	AActor* GetTargetActor() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	UDamageSphereComponent* DamageTrigger = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy|Damage")
	int32 AttackDamage = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy|Damage")
	FName AttackSocketName = "RightHandSocket";
};
