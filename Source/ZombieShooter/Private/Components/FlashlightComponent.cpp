// Created by Artyom Volkov during the UE4 course


#include "Components/FlashlightComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

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
	UGameplayStatics::PlaySound2D(GetWorld(), SwitchOnSound);
}

void UFlashlightComponent::Disable()
{
	if (bHiddenInGame) return;
	
	SetHiddenInGame(true);
	UGameplayStatics::PlaySound2D(GetWorld(), SwitchOffSound);
}
