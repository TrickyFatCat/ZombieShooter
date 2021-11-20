// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_FinishReload.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API UAnimNotify_FinishReload : public UAnimNotify
{
	GENERATED_BODY()

public:	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
