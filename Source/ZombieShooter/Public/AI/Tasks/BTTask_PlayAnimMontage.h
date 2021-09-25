// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TimerManager.h"
#include "BTTask_PlayAnimMontage.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API UBTTask_PlayAnimMontage : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_PlayAnimMontage();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimationMontage", meta=(ClampMin="0", ClampMax="1"))
	float Chance = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimationMontage")
	UAnimMontage* MontageToPlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimationMontage")
	float PlayRate = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimationMontage", meta=(ClampMin="0"))
	float RandomDeviation = 0.f;
	 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimationMontage", meta=(EditCondition="!bPlayRandomSection"))
	FName StartSectionName = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimationMontage")
	bool bPlayRandomSection = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimationMontage", meta=(EditCondition="bPlayRandomSection"))
	int32 SectionsNumber = 1;
	
	UPROPERTY()
	UBehaviorTreeComponent* MyOwnerComp;

	FTimerDelegate TimerDelegate;
	FTimerHandle TimerHandle;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void OnAnimationTimerDone();
};
