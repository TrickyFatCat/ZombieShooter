// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckLineOfSight.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API UBTService_CheckLineOfSight : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_CheckLineOfSight();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node")
	FBlackboardKeySelector TargetActorKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node")
	FBlackboardKeySelector IsInLineOfSightKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
