// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_GetActorLocation.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API UBTService_GetActorLocation : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_GetActorLocation();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node")
	FBlackboardKeySelector TargetActorKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node")
	FBlackboardKeySelector TargetActorLocationKey;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
