// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "WeaponCoreTypes.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class USkeletalMeshComponent;
class UWeaponFXComponent;

UCLASS()
class ZOMBIESHOOTER_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AWeaponBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	EWeaponType GetWeaponType() const { return WeaponData.WeaponType; }

	UFUNCTION(BlueprintPure, Category="Weapon")
	FName GetWeaponSocketName() const { return WeaponSocketName; }

	UFUNCTION(BlueprintGetter, Category="Weapon")
	FWeaponData GetWeaponData() const { return WeaponData; }

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess="true"))
	USceneComponent* WeaponRoot = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess="true"))
	USkeletalMeshComponent* WeaponMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess="true"))
	UWeaponFXComponent* WeaponFXComponent = nullptr;

	UPROPERTY(EditDefaultsOnly,
		BlueprintGetter=GetWeaponData,
		Category="Weapon",
		meta=(AllowPrivateAccess="true"))
	FWeaponData WeaponData;

	float TimeBetweenShots = 1.f;

	// Shooting
protected:
	FTimerHandle ShootingTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category="Weapon|Effects")
	UAnimationAsset* ShootAnimation = nullptr;

	UFUNCTION(BlueprintImplementableEvent, Category="Weapon")
	void OnWeaponShot();

	UFUNCTION(BlueprintImplementableEvent, Category="Weapon")
	void OnBulletShot(const FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);

	bool GetViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;

	bool GetTraceData(FVector& TraceStart, FVector& TraceEnd, const bool bCalculateSpread = true) const;

	void GetHitScanData(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintGetter=GetWeaponSocketName, Category="Weapon|Visuals", meta=(AllowPrivateAccess="true"))
	FName WeaponSocketName = FName("WeaponSocket");

	// Control
public:
	void StartShooting();
	void StopShooting();
	void Reload();

private:
	void MakeShot();

	// Ammo
public:
	FOnWeaponClipEmptySignature OnWeaponClipEmpty;

	bool CanShoot() const { return AmmoData.ClipAmmoCurrent > 0; }

	bool CanReload() const
	{
		return AmmoData.StorageAmmoCurrent > 0 && AmmoData.ClipAmmoCurrent < AmmoData.ClipAmmoMax;
	}

	bool StorageIsFull() const { return AmmoData.StorageAmmoCurrent >= AmmoData.StorageAmmoMax; }

	UFUNCTION(BlueprintGetter, Category="Weapon")
	FWeaponAmmoData GetAmmoData() const { return AmmoData; }

	void IncreaseCurrentStorageAmmo(const int32 Amount);
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintGetter=GetAmmoData, Category="Weapon")
	FWeaponAmmoData AmmoData;

	void IncreaseClipAmmoCurrent(const int32 Amount);

	void DecreaseClipAmmoCurrent(const int32 Amount);

	void DecreaseStorageAmmoCurrent(const int32 Amount);
};
