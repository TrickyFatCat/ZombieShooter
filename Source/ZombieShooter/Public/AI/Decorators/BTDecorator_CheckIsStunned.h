// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckIsStunned.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API UBTDecorator_CheckIsStunned : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_CheckIsStunned();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
