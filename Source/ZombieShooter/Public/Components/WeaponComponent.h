// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapons/WeaponBase.h"
#include "Weapons/WeaponCoreTypes.h"
#include "WeaponComponent.generated.h"

class AWeaponBase;
class UTimelineComponent;

UENUM()
enum class EWeaponPullCommand : uint8
{
	Equip,
	Reload
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ZOMBIESHOOTER_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime,
	                           ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintGetter, Category="Weapon")
	AWeaponBase* GetCurrentWeapon() const { return CurrentWeapon; }

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon", meta=(AllowPrivateAccess="true"))
	TArray<TSubclassOf<AWeaponBase>> WeaponClasses;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Weapon", meta=(AllowPrivateAccess="true"))
	TArray<AWeaponBase*> Weapons;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetCurrentWeapon)
	AWeaponBase* CurrentWeapon = nullptr;

	UPROPERTY(VisibleInstanceOnly)
	int32 CurrentWeaponIndex = 0;

	void SpawnWeapons();

	// Weapon control
public:
	void EquipNextWeapon();
	void EquipPreviousWeapon();
	void StartShooting();
	void StopShooting();
	void Reload();

protected:
	void EquipWeapon(const int32 WeaponIndex);
	void OnEmptyClip(AWeaponBase* TargetWeapon);

	// Pull animation
private:
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation", meta=(AllowPrivateAccess="true"))
	UCurveFloat* PullAnimationCurve = nullptr;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UTimelineComponent* PullAnimationTimeline = nullptr;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Animation", meta=(AllowPrivateAccess="true"))
	float PullProgress = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation", meta=(AllowPrivateAccess="true", ClampMin="0"))
	float EquipDuration = 0.25f;

	EWeaponPullCommand PullCommand = EWeaponPullCommand::Equip;

	UFUNCTION()
	void SetPullProgress(const float Value)
	{
		PullProgress = Value;
		UE_LOG(LogTemp, Warning, TEXT("%f"), Value);
	}

	UFUNCTION()
	void OnPullFinished();

	void StartEquipAnimation();
	
	// Weapon parameters
public:
	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool CanShoot() const { return CurrentWeapon->CanShoot() && !bIsReloading && !bIsEquipping; }

	UFUNCTION(BlueprintGetter, Category="Weapon")
	bool GetIsReloading() const { return bIsReloading; }

	UFUNCTION(BlueprintGetter, Category="Weapon")
	bool GetIsEquipping() const { return bIsEquipping; }

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void GetCurrentWeaponAmmo(FWeaponAmmoData& AmmoData) const;

protected:
private:
	UPROPERTY(BlueprintGetter=GetIsReloading, Category="Weapon")
	bool bIsReloading = false;

	UPROPERTY(BlueprintGetter=GetIsEquipping, Category="Weapon")
	bool bIsEquipping = false;
};
