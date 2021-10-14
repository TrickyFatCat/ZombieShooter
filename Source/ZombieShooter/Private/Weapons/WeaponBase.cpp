// Created by Artyom Volkov during the UE4 course

#include "Weapons/WeaponBase.h"
#include "GameFramework/Character.h"
#include "Weapons/Components/WeaponFXComponent.h"
#include "Core/ProjectUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/ProjectileBase.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponRoot = CreateDefaultSubobject<USceneComponent>("WeaponRoot");
	SetRootComponent(WeaponRoot);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	WeaponMesh->SetupAttachment(GetRootComponent());

	WeaponFXComponent = CreateDefaultSubobject<UWeaponFXComponent>("WeaponFXComponent");
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	TimeBetweenShots = WeaponData.RateOfFire <= 0.f ? 1.f : 1.f / WeaponData.RateOfFire;
	WeaponMesh->SetPlayRate(1.f / TimeBetweenShots);
	AmmoData.ClipAmmoCurrent = AmmoData.ClipAmmoMax;
	AmmoData.StorageAmmoCurrent = AmmoData.StorageAmmoMax;
	SetActorRelativeLocation(WeaponOffset);
}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponBase::GetWeaponData(FWeaponData& Data) const
{
	Data = this->WeaponData;
}

void AWeaponBase::SetSpreadMultiplier(const float Value)
{
	if (Value < 0.f) return;

	SpreadMultiplier = Value;
}

bool AWeaponBase::GetTraceData(FVector& TraceStart, FVector& TraceEnd, const bool bCalculateSpread) const
{
	FVector ViewLocation = FVector::ZeroVector;
	FRotator ViewRotation = FRotator::ZeroRotator;

	if (!FProjectUtils::GetPlayerViewPoint(GetOwner(), ViewLocation, ViewRotation)) return false;

	TraceStart = ViewLocation;
	FVector TraceDirection = ViewRotation.Vector();

	if (WeaponData.Spread > 0.f && bCalculateSpread)
	{
		const float SpreadAngleRad = FMath::DegreesToRadians(WeaponData.Spread / 2) * SpreadMultiplier;
		TraceDirection = FMath::VRandCone(TraceDirection, SpreadAngleRad);
	}

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

AController* AWeaponBase::GetOwnerController() const
{
	APawn* Pawn = Cast<APawn>(GetOwner());

	return Pawn ? Pawn->GetController() : nullptr;
}

void AWeaponBase::EnableShooting()
{
	bCanShoot = true;
}

void AWeaponBase::ApplyDamage(const FHitResult HitResult, const FVector& Direction)
{
	AActor* TargetActor = HitResult.GetActor();

	if (!TargetActor) return;

	UGameplayStatics::ApplyPointDamage(TargetActor,
	                                   CalculateDamage(),
	                                   Direction,
	                                   HitResult,
	                                   GetOwnerController(),
	                                   this,
	                                   WeaponData.DamageType);

	UMeshComponent* MeshComponent = Cast<UMeshComponent>(HitResult.Component);

	if (!TargetActor->IsA(APawn::StaticClass()))
	{
		MakeNoise(0.75, Cast<APawn>(GetOwner()), HitResult.ImpactPoint);
	}

	if (MeshComponent && MeshComponent->IsSimulatingPhysics())
	{
		MeshComponent->AddImpulse(Direction * WeaponData.HitScanImpulse);
	}
}

void AWeaponBase::StartShooting()
{
	if (!CanShoot()) return;

	MakeShot();
	GetWorldTimerManager().SetTimer(ShootingTimerHandle, this, &AWeaponBase::MakeShot, TimeBetweenShots, true);
}

void AWeaponBase::StopShooting()
{
	if (!GetWorldTimerManager().IsTimerActive(ShootingTimerHandle)) return;

	const float RemainingTime = GetWorldTimerManager().GetTimerRemaining(ShootingTimerHandle);
	GetWorldTimerManager().ClearTimer(ShootingTimerHandle);

	if (!CanShoot()) return;
	bCanShoot = false;
	GetWorldTimerManager().SetTimer(ShootingCooldownHandle, this, &AWeaponBase::EnableShooting, RemainingTime, false);
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
	FHitResult HitResult;

	for (int32 i = 1; i <= WeaponData.BulletsInShot; ++i)
	{
		if (!GetTraceData(TraceStart, TraceEnd, i != 1 || WeaponData.BulletsInShot == 1))
		{
			StopShooting();
			continue;
		}

		GetHitScanData(HitResult, TraceStart, TraceEnd);
		const FVector MuzzleLocation = WeaponMesh->GetSocketLocation(MuzzleSocketName);
		const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
		const FVector Direction = (EndPoint - MuzzleLocation).GetSafeNormal();

		if (WeaponData.BulletType == EBulletType::HitScan)
		{
			if (HitResult.bBlockingHit)
			{
				ApplyDamage(HitResult, Direction);
				WeaponFXComponent->PlayImpactFX(HitResult);
			}
		}
		else
		{
			const FTransform SpawnTransform(FRotator::ZeroRotator, MuzzleLocation);
			AProjectileBase* Projectile = GetWorld()->SpawnActorDeferred<AProjectileBase>(
				WeaponData.ProjectileClass,
				SpawnTransform);

			if (Projectile)
			{
				Projectile->SetDirectionAndDamage(Direction, WeaponData.DamageType, CalculateDamage());
				Projectile->SetOwner(GetOwner());
				Projectile->FinishSpawning(SpawnTransform);
			}
		}

		OnBulletShot(HitResult, TraceStart, HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd);
	}

	OnWeaponShot();
	OnMakeShot.Broadcast();
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
