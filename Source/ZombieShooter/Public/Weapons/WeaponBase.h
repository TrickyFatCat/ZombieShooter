// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "WeaponCoreTypes.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class USkeletalMeshComponent;
class UWeaponFXComponent;
class USoundCue;

DECLARE_MULTICAST_DELEGATE(FOnMakeShotSignature)

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

	UFUNCTION(BlueprintPure, Category="Weapon")
	void GetWeaponData(FWeaponData& Data) const;

	USkeletalMeshComponent* GetMesh() const { return WeaponMesh; }

	float GetTimeBetweenShots() const { return TimeBetweenShots; }

	FOnMakeShotSignature OnMakeShot;


private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	USceneComponent* WeaponRoot = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	USkeletalMeshComponent* WeaponMesh = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UWeaponFXComponent* WeaponFXComponent = nullptr;

	UPROPERTY(EditDefaultsOnly,
		Category="Weapon",
		meta=(AllowPrivateAccess="true"))
	FWeaponData WeaponData;

	float TimeBetweenShots = 1.f;

	// Shooting
public:
	float GetSpreadMultiplier() const { return SpreadMultiplier; }

	void SetSpreadMultiplier(const float Value);
protected:
	FTimerHandle ShootingTimerHandle;

	FTimerHandle ShootingCooldownHandle;

	UFUNCTION(BlueprintImplementableEvent, Category="Weapon")
	void OnWeaponShot();

	UFUNCTION(BlueprintImplementableEvent, Category="Weapon")
	void OnBulletShot(const FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);

	bool GetTraceData(FVector& TraceStart, FVector& TraceEnd, const bool bCalculateSpread = true) const;

	void GetHitScanData(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const;

	AController* GetOwnerController() const;

	int32 CalculateDamage() const { return FMath::Max(WeaponData.Damage / WeaponData.BulletsInShot, 1); }

private:
	bool bCanShoot = true;

	float SpreadMultiplier = 1.f;

	UFUNCTION()
	void EnableShooting();

	void ApplyDamage(const FHitResult HitResult, const FVector& Direction);

	// Control
public:
	void StartShooting();
	void StopShooting();
	void Reload();
	FOnReloadFinishedSignature OnReloadFinished;

private:
	void MakeShot();

	// Ammo
public:
	FOnWeaponClipEmptySignature OnWeaponClipEmpty;

	bool CanShoot() const { return AmmoData.ClipAmmoCurrent > 0 && bCanShoot; }

	bool CanReload() const
	{
		return AmmoData.StorageAmmoCurrent > 0 && AmmoData.ClipAmmoCurrent < AmmoData.ClipAmmoMax;
	}

	bool IsEmpty() const { return AmmoData.StorageAmmoCurrent == 0 && AmmoData.ClipAmmoCurrent == 0; }
	
	bool StorageIsFull() const { return AmmoData.StorageAmmoCurrent >= AmmoData.StorageAmmoMax; }

	bool IsClipEmpty() const { return AmmoData.ClipAmmoCurrent <= 0; }

	bool IsStorageEmpty() const { return AmmoData.StorageAmmoCurrent <= 0; }

	UFUNCTION(BlueprintGetter, Category="Weapon")
	FWeaponAmmoData GetAmmoData() const { return AmmoData; }

	void IncreaseCurrentStorageAmmo(const int32 Amount);
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintGetter=GetAmmoData, Category="Weapon")
	FWeaponAmmoData AmmoData;

	void IncreaseClipAmmoCurrent(const int32 Amount);

	void DecreaseClipAmmoCurrent(const int32 Amount);

	void DecreaseStorageAmmoCurrent(const int32 Amount);

	// Visuals and sounds
public:
	void PlayReloadAnimation() const;

	UAnimationAsset* GetReloadAnimation() const { return VisualData.ReloadAnimation; }

	FVector GetWeaponLocationOffset() const { return VisualData.WeaponLocationOffset; }

	FVector GetReloadLocationOffset() const { return VisualData.ReloadLocationOffset; }
	
	FRotator GetReloadRotationOffset() const { return VisualData.ReloadRotationOffset; }

	UTexture2D* GetIcon() const { return VisualData.Icon; }
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon", meta=(AllowPrivateAccess="true"))
	FWeaponVisualData VisualData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Sounds", meta=(AllowPrivateAccess="true"))
	USoundCue* EmptySound = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Sounds", meta=(AllowPrivateAccess="true"))
	USoundCue* ShotSound = nullptr;
};
