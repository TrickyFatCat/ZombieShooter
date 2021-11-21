// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_ResourceData.generated.h"

class UImage;
class UProgressBar;

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
	UProgressBar* ProgressBar_ResourceProgress = nullptr;
	
};
