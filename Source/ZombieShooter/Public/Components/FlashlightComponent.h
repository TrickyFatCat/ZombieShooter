// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Components/SpotLightComponent.h"
#include "FlashlightComponent.generated.h"

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
	void Enable();

	void Disable();
};
