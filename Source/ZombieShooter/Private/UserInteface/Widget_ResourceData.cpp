// Created by Artyom Volkov during the UE4 course


#include "UserInteface/Widget_ResourceData.h"

#include "Components/ProgressBar.h"
#include "Components/ShooterDamageControllerComponent.h"

void UWidget_ResourceData::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerPawn = GetOwningPlayerPawn();
	
	if (PlayerPawn)
	{
		DamageControllerComponent = PlayerPawn->FindComponentByClass<UShooterDamageControllerComponent>();
	}
}

int32 UWidget_ResourceData::GetCurrentHealth() const
{
	if (!DamageControllerComponent) return -1;

	return DamageControllerComponent->GetHealth();
}

int32 UWidget_ResourceData::GetCurrentArmor() const
{
	if (!DamageControllerComponent) return -1;

	return DamageControllerComponent->GetArmor();
}

float UWidget_ResourceData::GetNormalizedHealth() const
{
	if (!DamageControllerComponent) return -1;

	return DamageControllerComponent->GetNormalizedHealth();
}

float UWidget_ResourceData::GetNormalizedArmor() const
{
	if (!DamageControllerComponent) return -1;

	return DamageControllerComponent->GetNormalizedArmor();
}

float UWidget_ResourceData::CalculateHealthPercent(const float LerpAlpha) const
{
	if (!DamageControllerComponent) return -1;

	return FMath::Lerp(ProgressBar_ResourceProgress->Percent, DamageControllerComponent->GetNormalizedHealth(), LerpAlpha);
}

float UWidget_ResourceData::CalculateArmorPercent(const float LerpAlpha) const
{
	if (!DamageControllerComponent) return -1;
	
	return FMath::Lerp(ProgressBar_ResourceProgress->Percent, DamageControllerComponent->GetNormalizedArmor(), LerpAlpha);
}
