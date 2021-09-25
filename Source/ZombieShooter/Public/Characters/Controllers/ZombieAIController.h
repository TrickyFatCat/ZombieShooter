// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AI/ZombiePerceptionComponent.h"
#include "ZombieAIController.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API AZombieAIController : public AAIController
{
	GENERATED_BODY()

public:
	AZombieAIController();

protected:
	virtual void BeginPlay() override;
	
public:
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void OnPossess(APawn* InPawn) override;

	// Behaviour tree
public:
	AActor* GetTargetActor() const;
	
	void SetTargetActor(AActor* Value);
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="BehaviorTree", meta=(AllowPrivateAccess="true"))
	FName TargetActorKeyName = "TargetActor";
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="BehaviorTree", meta=(AllowPrivateAccess="true"))
	FName InitialLocationKeyName = "InitialLocation";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="BehaviorTree", meta=(AllowPrivateAccess="true"))
	FName IsPatrollingKeyName = "IsPatrolling";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="BehaviorTree", meta=(AllowPrivateAccess="true"))
	FName IsAttackingKeyName = "IsAttacking";
	

	// Perception
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	UZombiePerceptionComponent* Perception = nullptr;
};
