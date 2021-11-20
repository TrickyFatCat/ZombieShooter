// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_GetDistanceToActor.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API UBTService_GetDistanceToActor : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_GetDistanceToActor();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node") 
	FBlackboardKeySelector DistanceKey;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
