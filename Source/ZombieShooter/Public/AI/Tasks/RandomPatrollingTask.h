// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RandomPatrollingTask.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API URandomPatrollingTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	URandomPatrollingTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Patrolling")
	float Radius = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Patrolling")
	FBlackboardKeySelector InitialLocationKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Patrolling")
	FBlackboardKeySelector TargetLocationKey;
};
