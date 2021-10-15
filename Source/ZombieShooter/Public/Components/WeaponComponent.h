// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "Weapons/WeaponBase.h"
#include "Weapons/WeaponCoreTypes.h"
#include "WeaponComponent.generated.h"

class AWeaponBase;
class UCameraComponent;
class UMaterialParameterCollection;

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
	UPROPERTY(BlueprintAssignable, Category="Weapon")
	FOnWeaponShotSignature OnWeaponShot;

	void EquipNextWeapon();

	void EquipPreviousWeapon();

	void StartShooting();

	void StopShooting();

	void Reload();

	bool UnlockWeapon(TSubclassOf<AWeaponBase> WeaponClass);

	bool RestoreStorageAmmo(TSubclassOf<AWeaponBase> WeaponClass, const int32 Amount);

	UFUNCTION(BlueprintPure, Category="Weapon")
	bool GetIsShooting() const { return CanShoot() && !AdsTransitionTimeline->IsPlaying() && bIsShooting; }

protected:
	bool bIsShooting;

	FTimerHandle ReloadTimerHandle;

	void EquipWeapon(const int32 WeaponIndex);

	void OnEmptyClip(AWeaponBase* TargetWeapon);

	UFUNCTION()
	void OnReloadFinished();

private:
	const float WallCheckDistance = 80.f;
	
	void OnWeaponMakeShot();

	void CheckIsNearWall();

	// Recoil animation
public:
	UFUNCTION(BlueprintGetter, Category="Weapon|Animation")
	float GetRecoilProgress() const { return RecoilProgress; }

	UFUNCTION(BlueprintPure, Category="Weapon|Animation")
	bool GetIsRecoiling() const { return RecoilTimeline->IsPlaying(); }

private:
	float RecoilMultiplier = 1.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation|Recoil", meta=(AllowPrivateAccess="true"))
	UCurveFloat* RecoilAnimationCurve = nullptr;

	UPROPERTY()
	UTimelineComponent* RecoilTimeline = nullptr;

	UPROPERTY(VisibleInstanceOnly,
		BlueprintGetter=GetRecoilProgress,
		Category="Animation|Recoil",
		meta=(AllowPrivateAccess="true"))
	float RecoilProgress = 0.f;

	UPROPERTY(EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Animation|Recoil",
		meta=(AllowPrivateAccess="true", ClampMin="0"))
	float RecoilDuration = 0.2f;

	UFUNCTION()
	void SetRecoilOffset(const float Value);

	UFUNCTION()
	void OnRecoilFinished();

	// Pull animation
public:
	UFUNCTION(BlueprintPure, Category="Weapon|Animation")
	float GetPullProgress() const { return PullProgress; }

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation|Pull", meta=(AllowPrivateAccess="true"))
	UCurveFloat* PullAnimationCurve = nullptr;

	UPROPERTY()
	UTimelineComponent* PullAnimationTimeline = nullptr;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Animation|Pull", meta=(AllowPrivateAccess="true"))
	float PullProgress = 0.f;

	UPROPERTY(EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Animation|Pull",
		meta=(AllowPrivateAccess="true", ClampMin="0"))
	float PullDuration = 0.25f;

	UPROPERTY(EditDefaultsOnly, Category="Animation|Pull")
	FRotator EquipRotationOffset = FRotator(0.f, 0.f, 60.f);

	UPROPERTY(EditDefaultsOnly, Category="Animation|Pull")
	FVector EquipLocationOffset = FVector(0.f, 0.f, -40.f);

	EWeaponPullCommand PullCommand = EWeaponPullCommand::Equip;

	UFUNCTION()
	void SetPullOffset(const float Value);

	UFUNCTION()
	void OnPullFinished();

	void StartEquipAnimation();

	// Weapon parameters
public:
	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool CanShoot() const { return CurrentWeapon->CanShoot() && !bIsReloading && !bIsEquipping && !bIsNearWall && !AdsTransitionTimeline->IsPlaying(); }

	UFUNCTION(BlueprintGetter, Category="Weapon")
	bool GetIsReloading() const { return bIsReloading; }

	UFUNCTION(BlueprintGetter, Category="Weapon")
	bool GetIsEquipping() const { return bIsEquipping; }

	UFUNCTION(BlueprintGetter, Category="Weapon")
	bool GetIsNearWall() const { return bIsNearWall; }

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void GetCurrentWeaponAmmo(FWeaponAmmoData& AmmoData) const;

protected:
private:
	UPROPERTY(BlueprintGetter=GetIsReloading, Category="Weapon")
	bool bIsReloading = false;

	UPROPERTY(BlueprintGetter=GetIsEquipping, Category="Weapon")
	bool bIsEquipping = false;

	UPROPERTY(BlueprintGetter=GetIsNearWall, Category="Weapon")
	bool bIsNearWall = false;

	// ADS
public:
	UPROPERTY(BlueprintAssignable, Category="Weapon")
	FOnEnterAdsSignature OnEnterAds;

	UPROPERTY(BlueprintAssignable, Category="Weapon")
	FOnExitAdsSignature OnExitAds;
	
	UFUNCTION()
	void EnterAds();

	UFUNCTION()
	void ExitAds();

	UFUNCTION(BlueprintGetter, Category="Animation|ADS")
	float GetAdsTransitionProgress() const { return AdsTransitionProgress; }

	UFUNCTION(BlueprintPure, Category="Animation|ADS")
	bool GetIsUsingScope() const { return AdsData.bIsUsingScope; }

	UFUNCTION(BlueprintGetter, Category="Animation|ADS")
	bool GetIsAiming() const { return bIsAiming; }

	UFUNCTION(BlueprintPure, Category="Animation|ADS")
	FVector GetWeaponOffset() const { return AdsData.WeaponOffset; }

	float GetMovementFactor() const { return AdsData.MovementFactor; }

	float GetAimFactor() const { return AdsData.AimFactor; }

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation|ADS", meta=(AllowPrivateAccess="true"))
	UCurveFloat* AdsTransitionCurve = nullptr;

	UPROPERTY()
	UTimelineComponent* AdsTransitionTimeline = nullptr;

	UPROPERTY(VisibleInstanceOnly,
		BlueprintGetter=GetAdsTransitionProgress,
		Category="Animation|ADS",
		meta=(AllowPrivateAccess="true"))
	float AdsTransitionProgress = 0.f;

	UPROPERTY(EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Animation|ADS",
		meta=(AllowPrivateAccess="true", ClampMin="0"))
	float AdsTransitionDuration = 0.25f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation|ADS", meta=(AllowPrivateAccess="true"))
	UMaterialParameterCollection* PaniniParameterCollection = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation|ADS", meta=(AllowPrivateAccess="true"))
	FName FOVParameterName = FName("Global_FOV");

	float DefaultFOV = 90.f;

	UPROPERTY(BlueprintGetter=GetIsAiming)
	bool bIsAiming = false;

	FAdsData AdsData;

	UPROPERTY()
	UCameraComponent* TargetCamera = nullptr;

	UFUNCTION()
	void SetAdsTransitionProgress(const float Value);

	UFUNCTION()
	void OnAdsTransitionFinished();
};
