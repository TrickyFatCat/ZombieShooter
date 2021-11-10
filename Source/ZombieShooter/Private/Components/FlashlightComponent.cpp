// Created by Artyom Volkov during the UE4 course


#include "Components/FlashlightComponent.h"

void UFlashlightComponent::Switch()
{
	bHiddenInGame ? Enable() : Disable();
}

void UFlashlightComponent::BeginPlay()
{
	Super::BeginPlay();

	Disable();
}

void UFlashlightComponent::Enable()
{
	if (!bHiddenInGame) return;
	
	SetHiddenInGame(false);
}

void UFlashlightComponent::Disable()
{
	if (bHiddenInGame) return;
	
	SetHiddenInGame(true);
}
