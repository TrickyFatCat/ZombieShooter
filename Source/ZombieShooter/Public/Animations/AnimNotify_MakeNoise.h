// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_MakeNoise.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API UAnimNotify_MakeNoise : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_MakeNoise();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Noise")
	float Loudness = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Noise")
	float MaxRange = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Noise")
	FName Tag = NAME_None;
};
