// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_WeaponData.generated.h"

class UImage;
class UTextBlock;

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
};
