// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Components/SpotLightComponent.h"
#include "FlashlightComponent.generated.h"

class USoundCue;

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API UFlashlightComponent : public USpotLightComponent
{
	GENERATED_BODY()

public:
	UFlashlightComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void Switch();
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Flashlight", meta=(AllowPrivateAccess="true"))
	float MinIntensity = 2000.f;

	float MaxIntensity = 0.f;
	
	FHitResult TraceHit;
	FCollisionQueryParams CollisionQueryParams;

	bool bIsEnabled = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Flashlight", meta=(AllowPrivateAccess="true"))
	USoundCue* SwitchOnSound = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Flashlight", meta=(AllowPrivateAccess="true"))
	USoundCue* SwitchOffSound = nullptr;
	
	void Enable();

	void Disable();
};
