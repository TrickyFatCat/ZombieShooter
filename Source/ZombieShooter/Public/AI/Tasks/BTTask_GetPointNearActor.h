// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetPointNearActor.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API UBTTask_GetPointNearActor : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
    UBTTask_GetPointNearActor();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node")
	FBlackboardKeySelector TargetLocationKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node")
	float DistanceMin = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node")
	float DistanceMax = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node")
	float SearchRadius = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node")
	float SearchConeHalfAngle = 90.f;
};
