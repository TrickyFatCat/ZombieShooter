// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
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

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="BehaviorTree", meta=(AllowPrivateAccess="true"))
	FName TargetActorKeyName = "TargetActor";
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="BehaviorTree", meta=(AllowPrivateAccess="true"))
	FName InitialLocationKeyName = "InitialLocation";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="BehaviorTree", meta=(AllowPrivateAccess="true"))
	FName IsPatrollingKeyName = "IsPatrolling";
	
	AActor* GetTargetActor() const;
};
