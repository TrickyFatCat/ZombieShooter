// Created by Artyom Volkov

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "FootstepsAudioComponent.generated.h"

class USoundCue;
class UPhysicalMaterial;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBIESHOOTER_API UFootstepsAudioComponent : public UAudioComponent
{
	GENERATED_BODY()

public:	
	UFootstepsAudioComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void StartPlayingSound();

	UFUNCTION()
	void StopPlayingSound();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Sound|Footsteps")
	TMap<TEnumAsByte<EPhysicalSurface>, USoundCue*> FootstepsSounds;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Sound|Footsteps")
	TEnumAsByte<EPhysicalSurface> CurrentSurface = EPhysicalSurface::SurfaceType1;

	FHitResult TraceHit;
	
	void CheckSurface(FHitResult& HitResult) const;

	void SetCurrentSurface(const FHitResult& HitResult);
};
