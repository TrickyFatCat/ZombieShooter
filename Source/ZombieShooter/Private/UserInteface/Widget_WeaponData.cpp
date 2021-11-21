// Created by Artyom Volkov during the UE4 course


#include "UserInteface/Widget_WeaponData.h"

#include "Components/WeaponComponent.h"
#include "Kismet/GameplayStatics.h"

void UWidget_WeaponData::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UWidget_WeaponData::GetCurrentWeaponAmmoData(FWeaponAmmoData& WeaponAmmoData) const
{
	const UWeaponComponent* WeaponComponent = GetOwningPlayerPawn()->FindComponentByClass<UWeaponComponent>();

	if (!WeaponComponent) return;

	WeaponComponent->GetCurrentWeaponAmmo(WeaponAmmoData);
}

FString UWidget_WeaponData::FormatAmmoData(const int32 AmmoNumber, const int32 MaxLength)
{
	FString BulletsString = FString::FromInt(AmmoNumber);
	const int32 AddSymbolNumber = MaxLength - BulletsString.Len();

	if (AddSymbolNumber > 0)
	{
		BulletsString = FString::ChrN(AddSymbolNumber, '0').Append(BulletsString);
	}

	return BulletsString;
}
