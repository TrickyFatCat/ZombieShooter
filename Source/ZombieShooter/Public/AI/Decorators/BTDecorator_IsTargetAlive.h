// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsTargetAlive.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API UBTDecorator_IsTargetAlive : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_IsTargetAlive();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node")
	FBlackboardKeySelector TargetActorKey;
};
