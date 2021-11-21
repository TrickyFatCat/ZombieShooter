// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_ResourceData.generated.h"

class UImage;
class UProgressBar;
class UTextBlock;
class UShooterDamageControllerComponent;

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API UWidget_ResourceData : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

protected:
	UPROPERTY(meta=(BindWidget))
	UImage* Image_ResourceIcon = nullptr;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* Text_CurrentResource = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UProgressBar* ProgressBar_ResourceProgress = nullptr;

private:
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	APawn* PlayerPawn = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	UShooterDamageControllerComponent* DamageControllerComponent = nullptr;
	
	UFUNCTION(BlueprintPure, Category="UserInterface")
	int32 GetCurrentHealth() const;

	UFUNCTION(BlueprintPure, Category="UserInterface")
	int32 GetCurrentArmor() const;

	UFUNCTION(BlueprintCallable, Category="UserInterface")
	float GetNormalizedHealth() const;
	
	UFUNCTION(BlueprintCallable, Category="UserInterface")
	float GetNormalizedArmor() const;

	UFUNCTION(BlueprintPure, Category="UserInterface")
	float CalculateHealthPercent(const float LerpAlpha = 0.25f) const;
	
	UFUNCTION(BlueprintPure, Category="UserInterface")
	float CalculateArmorPercent(const float LerpAlpha = 0.25f) const;
};
