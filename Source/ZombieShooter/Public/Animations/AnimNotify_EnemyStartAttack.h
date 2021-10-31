// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Characters/EnemyCoreTypes.h"
#include "AnimNotify_EnemyStartAttack.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API UAnimNotify_EnemyStartAttack : public UAnimNotify
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	EEnemyAttackType AttackType;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
