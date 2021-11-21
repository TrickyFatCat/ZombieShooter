// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WeaponComponent.h"
#include "Weapons/WeaponCoreTypes.h"
#include "Widget_WeaponData.generated.h"

class UImage;
class UTextBlock;
class UWeaponComponent;

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API UWidget_WeaponData : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

protected:
	UPROPERTY(meta=(BindWidget))
	UImage* Image_WeaponIcon = nullptr;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* Text_ClipAmmo = nullptr;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* Text_InventoryAmmo = nullptr;

	UPROPERTY()
	UWeaponComponent* WeaponComponent = nullptr;

private:
	UFUNCTION(BlueprintPure, Category="UserInterface")
	void GetCurrentWeaponAmmoData(FWeaponAmmoData& WeaponAmmoData) const;

	UFUNCTION(BlueprintPure, Category="UserInterface")
	static FString FormatAmmoData(const int32 AmmoNumber, const int32 MaxLength);

	UFUNCTION()
	void SetWeaponIcon(const AWeaponBase* CurrentWeapon);
};