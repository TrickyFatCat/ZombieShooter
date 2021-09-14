// Created by Artyom Volkov during the UE4 course


#include "Weapons/WeaponBase.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponRoot = CreateDefaultSubobject<USceneComponent>("WeaponRoot");
	SetRootComponent(WeaponRoot);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	WeaponMesh->SetupAttachment(GetRootComponent());
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	TimeBetweenShots = WeaponData.RateOfFire <= 0.f ? 1.f : 1.f / WeaponData.RateOfFire;
	WeaponMesh->SetPlayRate(1.f / TimeBetweenShots);
	AmmoData.ClipAmmoCurrent = AmmoData.ClipAmmoMax;
	AmmoData.StorageAmmoCurrent = AmmoData.StorageAmmoMax;
}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::StartShooting()
{
	MakeShot();
	GetWorldTimerManager().SetTimer(ShootingTimerHandle, this, &AWeaponBase::MakeShot, TimeBetweenShots, true);
}

void AWeaponBase::StopShooting()
{
	GetWorldTimerManager().ClearTimer(ShootingTimerHandle);
	WeaponMesh->Stop();
}

void AWeaponBase::Reload()
{
	if (AmmoData.StorageAmmoCurrent <= 0 && !AmmoData.bIsStorageInfinite || AmmoData.bIsClipInfinite) return;

	StopShooting();

	if (AmmoData.bIsStorageInfinite)
	{
		IncreaseClipAmmoCurrent(AmmoData.ClipAmmoMax);
		return;
	}

	const int32 RequiredAmmo = AmmoData.ClipAmmoMax - AmmoData.ClipAmmoCurrent;
	const int32 DeltaAmmo = RequiredAmmo > AmmoData.StorageAmmoCurrent ? AmmoData.StorageAmmoCurrent : RequiredAmmo;
	IncreaseClipAmmoCurrent(DeltaAmmo);
	DecreaseStorageAmmoCurrent(DeltaAmmo);
}

void AWeaponBase::MakeShot()
{
	if (AmmoData.ClipAmmoCurrent <= 0)
	{
		StopShooting();
		return;
	}
	UE_LOG(LogTemp, Error, TEXT("Pew"));

	OnWeaponShot();
	WeaponMesh->PlayAnimation(ShootAnimation, false);
	DecreaseClipAmmoCurrent(WeaponData.ShotCost);
}

void AWeaponBase::IncreaseCurrentStorageAmmo(const int32 Amount)
{
	if (Amount <= 0 || AmmoData.StorageAmmoCurrent >= AmmoData.StorageAmmoMax) return;

	AmmoData.StorageAmmoCurrent += Amount;
	AmmoData.StorageAmmoCurrent = FMath::Min(AmmoData.StorageAmmoCurrent, AmmoData.StorageAmmoMax);
}

void AWeaponBase::IncreaseClipAmmoCurrent(const int32 Amount)
{
	if (Amount <= 0 || AmmoData.ClipAmmoCurrent >= AmmoData.ClipAmmoMax) return;

	AmmoData.ClipAmmoCurrent += Amount;
	AmmoData.ClipAmmoCurrent = FMath::Min(AmmoData.ClipAmmoCurrent, AmmoData.ClipAmmoMax);
}

void AWeaponBase::DecreaseClipAmmoCurrent(const int32 Amount)
{
	if (Amount <= 0 || AmmoData.ClipAmmoCurrent <= 0) return;

	AmmoData.ClipAmmoCurrent -= Amount;
	AmmoData.ClipAmmoCurrent = FMath::Max(AmmoData.ClipAmmoCurrent, 0);

	if (AmmoData.ClipAmmoCurrent <= 0 && AmmoData.StorageAmmoCurrent > 0)
	{
		OnWeaponClipEmpty.Broadcast(this);
	}
}

void AWeaponBase::DecreaseStorageAmmoCurrent(const int32 Amount)
{
	if (Amount <= 0 || AmmoData.StorageAmmoCurrent <= 0) return;

	AmmoData.StorageAmmoCurrent -= Amount;
	AmmoData.StorageAmmoCurrent = FMath::Max(AmmoData.StorageAmmoCurrent, 0);
}

