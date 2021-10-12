// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AI/AICoreTypes.h"
#include "AIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()

public:
	AAIControllerBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void OnPossess(APawn* InPawn) override;

	// Blackboard Keys
protected:
	UPROPERTY(EditDefaultsOnly, Category="AI|BlackboardKeys")
	FName GeneralStateKeyName = FName("GeneralState");
	
	UPROPERTY(EditDefaultsOnly, Category="AI|BlackboardKeys")
	FName SpawnLocationKeyName = FName("SpawnLocation");

	UPROPERTY(EditDefaultsOnly, Category="AI|BlackboardKeys")
	FName TargetActorKeyName = FName("TargetActor");
	
	UPROPERTY(EditDefaultsOnly, Category="AI|BlackboardKeys")
	FName InvestigationLocationKeyName = FName("InvestigationLocation");

	// State
public:
	UFUNCTION(BlueprintPure, Category="AI|State")
	EEnemyGeneralState GetCurrentGeneralState() const;

	void SetGeneralState(const EEnemyGeneralState NewState) const;

private:
	void SetTargetActor(AActor* TargetActor) const;

	void SetInvestigationLocation(FVector Location) const;

	void StartInvestigation(const FVector Location);

	// Perception
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	UAIPerceptionComponent* Perception = nullptr;

	UFUNCTION()
	virtual void OnPerceptionUpdated(const TArray<AActor*>& Actors);

	// Other
public:
	void AttackPlayer() const;
};
