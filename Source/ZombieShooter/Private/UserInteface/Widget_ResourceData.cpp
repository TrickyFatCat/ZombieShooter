// Created by Artyom Volkov during the UE4 course


#include "UserInteface/Widget_ResourceData.h"

#include "Components/ShooterDamageControllerComponent.h"

void UWidget_ResourceData::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (GetOwningPlayerPawn())
	{
		DamageControllerComponent = GetOwningPlayerPawn()->FindComponentByClass<UShooterDamageControllerComponent>();
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
