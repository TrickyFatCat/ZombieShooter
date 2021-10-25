// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_TargetIsNear.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API UBTDecorator_TargetIsNear : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_TargetIsNear();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, Category="Node")
	FBlackboardKeySelector TargetActorKey;
	
	UPROPERTY(EditAnywhere, Category="Node")
	FBlackboardKeySelector DistanceToActorKey;

	UPROPERTY(EditAnywhere, Category="Node")
	float TargetDistance = 200.f;
};
