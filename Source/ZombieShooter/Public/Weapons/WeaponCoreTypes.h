// Created by Artyom Volkov during the UE4 course

#pragma once

#include "WeaponCoreTypes.generated.h"

class AWeaponBase;
class AProjectileBase;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnWeaponClipEmptySignature, AWeaponBase*)

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
struct FWeaponSpawnData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category="Weapon")
	TSubclassOf<AWeaponBase> WeaponClass = nullptr;

	UPROPERTY(EditAnywhere, Category="Weapon")
	UTexture2D* WeaponIcon = nullptr;
};

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	EWeaponType WeaponType = EWeaponType::Pistol;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FName WeaponSocketName = NAME_None;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
	int32 Damage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
	float RateOfFire = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
	float Spread = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon", meta=(ClampMin="0"))
	int32 ShotCost = 1; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
	EBulletType BulletType = EBulletType::HitScan;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon", meta=(EditCondition="BulletType==EBulletType::Projectile"))
	TSubclassOf<AProjectileBase> ProjectileClass = nullptr;
};

USTRUCT(BlueprintType)
struct FWeaponAmmoData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ammo")
	EWeaponType AmmoType = EWeaponType::Pistol;

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
