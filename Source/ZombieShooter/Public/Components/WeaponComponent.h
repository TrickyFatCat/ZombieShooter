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

	UFUNCTION(BlueprintPure, Category="Weapon")
	void GetCurrentWeaponData(FWeaponData& WeaponData) const;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon", meta=(AllowPrivateAccess="true"))
	TArray<TSubclassOf<AWeaponBase>> WeaponClasses;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Weapon", meta=(AllowPrivateAccess="true"))
	TArray<FWeaponInventoryData> Weapons;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetCurrentWeapon)
	AWeaponBase* CurrentWeapon = nullptr;

	UPROPERTY(VisibleInstanceOnly)
	int32 CurrentWeaponIndex = 0;

	UPROPERTY(VisibleInstanceOnly)
	int32 PreviousWeaponIndex = 0;

	void SpawnWeapons();

	// Weapon control
public:
	void EquipNextWeapon();

	void EquipPreviousWeapon();

	void StartShooting();

	void StopShooting();

	void Reload();

	void UnlockWeapon(TSubclassOf<AWeaponBase> WeaponClass);

	bool RestoreStorageAmmo(TSubclassOf<AWeaponBase> WeaponClass, const int32 Amount);

	UFUNCTION(BlueprintPure, Category="Weapon")
	bool GetIsShooting() const { return CanShoot() && bIsShooting; }

protected:
	bool bIsShooting;

	FTimerHandle ReloadTimerHandle;

	void EquipWeapon(const int32 WeaponIndex);

	void OnEmptyClip(AWeaponBase* TargetWeapon);

	void OnReloadFinished() const;

	// Pull animation
public:
	UFUNCTION(BlueprintPure, Category="Animation")
	float GetPullProgress() const { return PullProgress; }

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation", meta=(AllowPrivateAccess="true"))
	UCurveFloat* PullAnimationCurve = nullptr;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UTimelineComponent* PullAnimationTimeline = nullptr;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Animation", meta=(AllowPrivateAccess="true"))
	float PullProgress = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation", meta=(AllowPrivateAccess="true", ClampMin="0"))
	float PullDuration = 0.25f;

	EWeaponPullCommand PullCommand = EWeaponPullCommand::Equip;

	UFUNCTION()
	void SetPullProgress(const float Value) { PullProgress = Value; }

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
