// Created by Artyom Volkov

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify_PlaySound.h"
#include "AnimNotify_PlayFootstepSound.generated.h"

class USoundCue;
/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API UAnimNotify_PlayFootstepSound : public UAnimNotify_PlaySound
{
	GENERATED_BODY()

public:
	UAnimNotify_PlayFootstepSound();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

protected:
	UPROPERTY(EditAnywhere)
	FName TargetBoneName = NAME_None;
	
	UPROPERTY(EditAnywhere)
	TMap<TEnumAsByte<EPhysicalSurface>, USoundCue*> FootstepSounds;

	TEnumAsByte<EPhysicalSurface> CurrentSurface = EPhysicalSurface::SurfaceType_Default;

	FHitResult TraceHit;

	virtual void UpdateCurrentSurface(UWorld* World, const FVector& TraceStart);
};
