// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/EnemyCoreTypes.h"
#include "AI/AICoreTypes.h"
#include "Components/TriggerComponents/DamageCapsuleComponent.h"
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
	UPROPERTY(EditDefaultsOnly, Category="Enemy|Death")
	UAnimMontage* DeathAnimMontage = nullptr;

	UPROPERTY(VisibleDefaultsOnly, Category="Enemy|Death")
	TArray<FName> DeathAnimMontageSections = {FName("Death_Front"), FName("Death_Back")};

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Enemy|HitReaction")
	int32 SectionsNumber = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy|HitReaction")
	TMap<TSubclassOf<UDamageType>, float> StunChancesMap;

	UFUNCTION()
	virtual void OnTakeDamage(AActor* DamageActor,
	                          float Damage,
	                          const UDamageType* DamageType,
	                          AController* InstigatedBy,
	                          AActor* DamageCauser);

	// Attack
public:
	virtual void StartAttack(const EEnemyAttackType AttackType);

	virtual void FinishAttack();

	AActor* GetTargetActor() const;

protected:
	EEnemyAttackType CurrentAttackType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UDamageCapsuleComponent* DamageTriggerRight = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UDamageCapsuleComponent* DamageTriggerLeft = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UDamageCapsuleComponent* DamageTriggerMouth = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy|Attack|Melee")
	int32 AttackDamage = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy|Attack|Melee")
	FName RightHandSocketName = "RightHandSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy|Attack|Melee")
	FName LeftHandSocketName = "LeftHandSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy|Attack|Melee")
	FName MouthSocketName = "MouthSocket";

	// Visuals
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy|Visuals")
	TArray<USkeletalMesh*> EnemyMeshes;

private:
	void SetRandomMesh();
};
