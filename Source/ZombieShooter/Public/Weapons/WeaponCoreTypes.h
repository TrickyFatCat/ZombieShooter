// Created by Artyom Volkov during the UE4 course

#pragma once

#include "WeaponCoreTypes.generated.h"

class AWeaponBase;
class AProjectileBase;
class USoundCue;
class UAnimationAsset;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnWeaponClipEmptySignature, AWeaponBase*)

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponShotSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnterAdsSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExitAdsSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReloadFinishedSignature);

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Recoil")
	FVector MeshLocationOffset = FVector(0.f, -10.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Recoil")
	FRotator MeshRotationOffset = FRotator(0.f, 0.f, -10.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Recoil", meta=(ClampMin="0"))
	float CameraRecoilPitchPower = 0.15f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Recoil", meta=(ClampMin="0"))
	float CameraRecoilYawPower = 0.15f;
};

USTRUCT(BlueprintType)
struct FAdsData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ADS")
	bool bHasAds = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ADS", meta=(EditCondition="bHasAds"))
	bool bIsUsingScope = false;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="ADS",
		meta=(EditCondition="bHasAds", ClampMin="0", ClampMax="180"))
	float TargetFOV = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ADS", meta=(EditCondition="bHasAds", ClampMin="0", ClampMax="1"))
	float RecoilFactor = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ADS", meta=(EditCondition="bHasAds", ClampMin="0", ClampMax="1"))
	float MovementFactor = 0.35f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ADS", meta=(EditCondition="bHasAds", ClampMin="0", ClampMax="1"))
	float SpreadFactor = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ADS", meta=(EditCondition="bHasAds", ClampMin="0", ClampMax="1"))
	float AimFactor = 0.25f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ADS", meta=(EditCondition="bHasAds"))
	FVector WeaponOffset = FVector::ZeroVector;
};

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
	TSubclassOf<UDamageType> DamageType = nullptr;

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
	FRecoilData RecoilData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Bullets", meta=(ClampMin="1", ClampMax="20"))
	int32 BulletsInShot = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Bullets")
	EBulletType BulletType = EBulletType::HitScan;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Bullets",
		meta=(EditCondition="BulletType==EBulletType::HitScan"))
	float HitScanDistance = 10000.f;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Bullets",
		meta=(EditCondition="BulletType==EBulletType::HitScan"))
	float HitScanImpulse = 10000.f;
	
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Bullets",
		meta=(EditCondition="BulletType==EBulletType::Projectile"))
	TSubclassOf<AProjectileBase> ProjectileClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
	FAdsData AdsData;
};

USTRUCT(BlueprintType)
struct FWeaponVisualData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Visuals")
	FName MuzzleSocketName = FName("MuzzleSocket");
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Visuals")
	FVector WeaponLocationOffset = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Visuals")
	UAnimationAsset* FireAnimation = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Visuals")
	UAnimationAsset* ReloadAnimation = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Visuals")
	FVector ReloadLocationOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Visuals")
	FRotator ReloadRotationOffset = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Visuals")
	UTexture2D* Icon = nullptr;
};

USTRUCT(BlueprintType)
struct FWeaponAmmoData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ammo")
	bool bIsClipInfinite = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Ammo")
	int32 ClipAmmoCurrent = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ammo")
	int32 ClipAmmoMax = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ammo")
	bool bIsStorageInfinite = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Ammo")
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

USTRUCT(BlueprintType)
struct FProjectileData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category="Projectile")
	TSubclassOf<UDamageType> DamageType = nullptr;

	UPROPERTY(BlueprintReadOnly, Category="Projectile", meta=(ClampMin="0"))
	int32 Damage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile")
	bool bIsBouncing = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile")
	bool bIsExplosive = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile", meta=(EditCondition="bIsExplosive", ClampMin="0"))
	float ExplosionRadius = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile", meta=(EditCondition="bIsExplosive"))
	bool bDealFullDamage = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile", meta=(EditCondition="bIsExplosive"))
	bool bDamageOwner = false;
};
