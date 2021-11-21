// Created by Artyom Volkov during the UE4 course


#include "UserInteface/Widget_WeaponData.h"

#include "Components/Image.h"
#include "Components/WeaponComponent.h"

void UWidget_WeaponData::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	WeaponComponent = GetOwningPlayerPawn()->FindComponentByClass<UWeaponComponent>();

	if (WeaponComponent)
	{
		WeaponComponent->OnWeaponChanged.AddDynamic(this, &UWidget_WeaponData::SetWeaponIcon);
	}
}

void UWidget_WeaponData::GetCurrentWeaponAmmoData(FWeaponAmmoData& WeaponAmmoData) const
{
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

void UWidget_WeaponData::SetWeaponIcon(const AWeaponBase* CurrentWeapon)
{
	if (!CurrentWeapon) return;

	Image_WeaponIcon->SetBrushFromTexture(CurrentWeapon->GetIcon());
}