// Created by Artyom Volkov during the UE4 course

#include "Weapons/WeaponBase.h"
#include "GameFramework/Character.h"

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

bool AWeaponBase::GetViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (!Character) return false;

	if (Character->IsPlayerControlled())
	{
		const APlayerController* Controller = Character->GetController<APlayerController>();

		if (!Controller) return false;

		Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	}

	return true;
}

bool AWeaponBase::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation = FVector::ZeroVector;
	FRotator ViewRotation = FRotator::ZeroRotator;

	if (!GetViewPoint(ViewLocation, ViewRotation)) return false;

	TraceStart = ViewLocation;
	const FVector TraceDirection = ViewRotation.Vector();
	TraceEnd = TraceStart + TraceDirection * WeaponData.HitScanDistance;
	return true;
}

void AWeaponBase::GetHitScanData(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const
{
	if (!GetWorld()) return;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());
	CollisionQueryParams.bReturnPhysicalMaterial = true;

	GetWorld()->LineTraceSingleByChannel(HitResult,
	                                     TraceStart,
	                                     TraceEnd,
	                                     ECollisionChannel::ECC_Visibility,
	                                     CollisionQueryParams);
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

	FVector TraceStart, TraceEnd;

	if (!GetTraceData(TraceStart, TraceEnd))
	{
		StopShooting();
		return;
	}

	FHitResult HitResult;
	GetHitScanData(HitResult, TraceStart, TraceEnd);

	if (WeaponData.BulletType == EBulletType::HitScan)
	{
		UE_LOG(LogTemp, Error, TEXT("Pew"));
		if (HitResult.bBlockingHit)
		{
			// Deal damage;
			// Play impact fx
		}
	}


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
