// Created by Artyom Volkov during the UE4 course

#pragma once

#include "WeaponCoreTypes.generated.h"

class AWeaponBase;
class AProjectileBase;
class USoundCue;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnWeaponClipEmptySignature, AWeaponBase*)

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponShotSignature);

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Pistol,
	AssaultRifle,
	Shotgun,
	GrenadeLauncher,
	RocketLauncher,
	SniperRifle
};

UENUM(BlueprintType)
enum class EBulletType : uint8
{
	HitScan,
	Projectile
};

USTRUCT(BlueprintType)
struct FWeaponInventoryData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category="Weapon")
	AWeaponBase* Weapon = nullptr;

	UPROPERTY(EditAnywhere, Category="Weapon")
	bool bIsAvailable = false;
};

USTRUCT(BlueprintType)
struct FRecoilData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Recoil", meta=(ClampMin="0"))
	float MeshRecoilYPower = 5.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Recoil", meta=(ClampMin="0"))
	float MeshRecoilRollPower = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Recoil", meta=(ClampMin="0", ClampMax="1"))
	float CameraRecoilPitchPower = 0.15f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Recoil", meta=(ClampMin="0", ClampMax="1"))
	float CameraRecoilYawPower = 0.15f;
};

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	EWeaponType WeaponType = EWeaponType::Pistol;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
	int32 Damage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
	float RateOfFire = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon", meta=(ClampMin="0", ClampMax="180"))
	float Spread = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon", meta=(ClampMin="0"))
	int32 ShotCost = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon", meta=(ClampMin="0"))
	float ReloadTime = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
	FRecoilData Recoil;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Bullets")
	EBulletType BulletType = EBulletType::HitScan;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Bullets", meta=(ClampMin="1", ClampMax="20"))
	int32 BulletsInShot = 1;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Bullets",
		meta=(EditCondition="BulletType==EBulletType::HitScan"))
	float HitScanDistance = 10000.f;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Bullets",
		meta=(EditCondition="BulletType==EBulletType::Projectile"))
	TSubclassOf<AProjectileBase> ProjectileClass = nullptr;
};

USTRUCT(BlueprintType)
struct FWeaponAmmoData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ammo")
	bool bIsClipInfinite = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ammo")
	int32 ClipAmmoCurrent = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ammo")
	int32 ClipAmmoMax = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ammo")
	bool bIsStorageInfinite = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ammo")
	int32 StorageAmmoCurrent = 300;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ammo")
	int32 StorageAmmoMax = 300;
};

USTRUCT(BlueprintType)
struct FHitScanDecalData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Decal")
	UMaterialInterface* Material;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Decal")
	FVector Size = FVector(16.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Decal")
	float LifeTime = 5.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Decal")
	float FadeOutDuration = 0.5f;
};

USTRUCT(BlueprintType)
struct FHitScanImpactData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="VFX")
	UParticleSystem* ImpactEffect = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SFX")
	USoundCue* Sound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="VFX")
	FHitScanDecalData DecalData;
};
