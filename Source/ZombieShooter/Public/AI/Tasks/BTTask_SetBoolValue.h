// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetBoolValue.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API UBTTask_SetBoolValue : public UBTTaskNode
{
	GENERATED_BODY()

	public:
	UBTTask_SetBoolValue();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node")
	FBlackboardKeySelector BoolValueKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node")
	bool bNewValue = true;
};
