// Created by Artyom Volkov during the UE4 course

#pragma once

#include "WeaponCoreTypes.generated.h"

class AWeaponBase;

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
struct FWeaponTypeData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category="Weapon")
	EWeaponType WeaponType = EWeaponType::Pistol;

	UPROPERTY(EditAnywhere, Category="Weapon")
	FName WeaponSocketName = NAME_None;
};