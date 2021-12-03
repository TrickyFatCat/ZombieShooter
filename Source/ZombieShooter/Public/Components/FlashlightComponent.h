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
	UFUNCTION()
	void Switch();

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Flashlight", meta=(AllowPrivateAccess="true"))
	USoundCue* SwitchOnSound = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Flashlight", meta=(AllowPrivateAccess="true"))
	USoundCue* SwitchOffSound = nullptr;
	
	void Enable();

	void Disable();
};
