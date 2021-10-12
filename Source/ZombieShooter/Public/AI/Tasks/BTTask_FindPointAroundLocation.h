// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindPointAroundLocation.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API UBTTask_FindPointAroundLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FindPointAroundLocation();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Patrolling")
	float Radius = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Patrolling")
	FBlackboardKeySelector PointKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Patrolling")
	FBlackboardKeySelector TargetLocationKey;
};
