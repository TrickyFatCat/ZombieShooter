// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_GetPointNearActor.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API UBTService_GetPointNearActor : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_GetPointNearActor();

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node")
	bool bCheckLineOfSight = false;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
