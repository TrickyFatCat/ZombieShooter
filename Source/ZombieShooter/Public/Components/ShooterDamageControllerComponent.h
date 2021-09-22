// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Components/DamageControllerComponent.h"
#include "ShooterDamageControllerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnArmorChangedSignature, float, NewArmor, float, DeltaArmor);

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API UShooterDamageControllerComponent : public UDamageControllerComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	// Armor
public:
	UPROPERTY()
	FOnArmorChangedSignature OnArmorChanged;

	UFUNCTION(BlueprintCallable, Category="Armor")
	float GetArmor() const { return ArmorObject->GetValue(); }

	UFUNCTION(BlueprintCallable, Category="Armor")
	float GetMaxArmor() const { return ArmorObject->GetValueMax(); }

	UFUNCTION(BlueprintCallable, Category="Armor")
	float GetNormalizedArmor() const { return ArmorObject->GetNormalizedValue(); }

	UFUNCTION(BlueprintCallable, Category="Armor")
	void DecreaseArmor(const float Amount, AController* Instigator);

	UFUNCTION(BlueprintCallable, Category="Armor")
	void IncreaseArmor(const float Amount, const bool bClampToMax = true);

	UFUNCTION(BlueprintCallable, Category="Armor")
	void DecreaseMaxArmor(const float Amount, const bool bClampCurrentArmor = true);

	UFUNCTION(BlueprintCallable, Category="Armor")
	void IncreaseMaxArmor(const float Amount, const bool bClampCurrentArmor = true);

	void BroadcastOnArmorChanged(const float NewArmor, const float DeltaArmor);

	UFUNCTION(BlueprintGetter, Category="Armor")
	float GetArmorModifier() const { return ArmorModifier; }

	UFUNCTION(BlueprintSetter, Category="Armor")
	void SetArmorModifier(const float Value);

protected:
	UPROPERTY(EditDefaultsOnly,
		BlueprintGetter=GetArmorModifier,
		BlueprintSetter=SetArmorModifier,
		meta=(AllowPrivateAccess="true",
		ClampMin="0"))
	float ArmorModifier = 0.25;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="DamageController", meta=(AllowPrivateAccess="true"))
	FResourceData ArmorData;

	UPROPERTY()
	UEntityResource* ArmorObject = nullptr;

	// Damage
	virtual void CalculateDamage(const float Damage, AActor* DamagedActor, AController* Instigator, AActor* Causer, const UDamageType* DamageType) override;
};
