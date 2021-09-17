// Created by Artyom Volkov during the UE4 course


#include "Components/WeaponComponent.h"

#include "Characters/PlayerCharacter.h"
#include "Components/TimelineComponent.h"
#include "Weapons/WeaponBase.h"
#include "Core/ProjectUtils.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	PullAnimationTimeline = CreateDefaultSubobject<UTimelineComponent>("PullAnimationTimeline");
}


void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapons();
	Weapons[0].bIsAvailable = true;
	EquipWeapon(CurrentWeaponIndex);

	PullAnimationTimeline->SetPlayRate(1.f / (PullDuration * 0.5f));

	if (PullAnimationCurve)
	{
		FOnTimelineFloat AnimationProgress;
		AnimationProgress.BindUFunction(this, FName("SetPullProgress"));
		PullAnimationTimeline->AddInterpFloat(PullAnimationCurve, AnimationProgress);

		FOnTimelineEvent AnimationFinished;
		AnimationFinished.BindUFunction(this, FName("OnPullFinished"));
		PullAnimationTimeline->SetTimelineFinishedFunc(AnimationFinished);
	}
}


void UWeaponComponent::TickComponent(float DeltaTime,
                                     ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckIsNearWall();
}

void UWeaponComponent::GetCurrentWeaponData(FWeaponData& WeaponData) const
{
	CurrentWeapon->GetWeaponData(WeaponData);
}

void UWeaponComponent::SpawnWeapons()
{
	if (!GetWorld() || !GetOwner()) return;

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());

	if (!PlayerCharacter) return;

	for (auto WeaponClass : WeaponClasses)
	{
		AWeaponBase* Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass);

		if (!Weapon) continue;

		Weapon->OnWeaponClipEmpty.AddUObject(this, &UWeaponComponent::OnEmptyClip);
		Weapon->SetOwner(GetOwner());
		Weapons.Add(FWeaponInventoryData{Weapon, false});
		FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
		Weapon->AttachToComponent(PlayerCharacter->GetPlayerArms(),
		                          AttachmentTransformRules,
		                          Weapon->GetWeaponSocketName());
		Weapon->SetActorHiddenInGame(true);
		Weapon->OnMakeShot.AddUObject(this, &UWeaponComponent::BroadCastOnWeaponShot);
	}
}

void UWeaponComponent::EquipNextWeapon()
{
	if (bIsEquipping || bIsReloading) return;

	PreviousWeaponIndex = CurrentWeaponIndex;

	do
	{
		CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	}
	while (!Weapons[CurrentWeaponIndex].bIsAvailable);

	if (PreviousWeaponIndex == CurrentWeaponIndex) return;

	CurrentWeapon->StopShooting();
	StartEquipAnimation();
}

void UWeaponComponent::EquipPreviousWeapon()
{
	if (bIsEquipping || bIsReloading) return;

	PreviousWeaponIndex = CurrentWeaponIndex;

	do
	{
		CurrentWeaponIndex = CurrentWeaponIndex == 0 ? Weapons.Num() - 1 : CurrentWeaponIndex - 1;
	}
	while (!Weapons[CurrentWeaponIndex].bIsAvailable);

	if (PreviousWeaponIndex == CurrentWeaponIndex) return;

	CurrentWeapon->StopShooting();
	StartEquipAnimation();
}

void UWeaponComponent::StartShooting()
{
	if (!CanShoot()) return;

	CurrentWeapon->StartShooting();
	bIsShooting = true;
}

void UWeaponComponent::StopShooting()
{
	if (!CurrentWeapon) return;

	CurrentWeapon->StopShooting();
	bIsShooting = false;
}

void UWeaponComponent::Reload()
{
	if (!CurrentWeapon || bIsEquipping || bIsReloading || !CurrentWeapon->CanReload()) return;

	CurrentWeapon->StopShooting();
	bIsReloading = true;
	PullCommand = EWeaponPullCommand::Reload;
	FWeaponData WeaponData;
	CurrentWeapon->GetWeaponData(WeaponData);

	if (WeaponData.ReloadTime <= 0.f)
	{
		CurrentWeapon->Reload();
		bIsReloading = false;
		return;
	}

	PullAnimationTimeline->PlayFromStart();
}

bool UWeaponComponent::UnlockWeapon(TSubclassOf<AWeaponBase> WeaponClass)
{
	bool Result = false;

	for (int32 i = 0; i < Weapons.Num(); ++i)
	{
		FWeaponInventoryData& InventoryData = Weapons[i];
		if (!InventoryData.Weapon->IsA(WeaponClass)) continue;

		if (InventoryData.bIsAvailable)
		{
			Result = false;
			break;
		};

		InventoryData.bIsAvailable = true;

		if (!bIsReloading && !bIsEquipping)
		{
			PreviousWeaponIndex = CurrentWeaponIndex;
			CurrentWeaponIndex = i;
			StartEquipAnimation();
		}

		Result = true;
		break;
	}

	return Result;
}

bool UWeaponComponent::RestoreStorageAmmo(TSubclassOf<AWeaponBase> WeaponClass, const int32 Amount)
{
	bool Result = false;

	if (Amount <= 0) return Result;

	for (int32 i = 0; i < Weapons.Num(); ++i)
	{
		const FWeaponInventoryData& InventoryData = Weapons[i];

		if (!InventoryData.Weapon->IsA(WeaponClass)) continue;

		if (InventoryData.Weapon->StorageIsFull())
		{
			Result = false;
			break;
		}

		InventoryData.Weapon->IncreaseCurrentStorageAmmo(Amount);
		Result = true;
		break;
	}

	return Result;
}


void UWeaponComponent::EquipWeapon(const int32 WeaponIndex)
{
	if (!GetOwner() || WeaponIndex >= Weapons.Num() || Weapons.Num() == 0) return;

	if (CurrentWeapon)
	{
		CurrentWeapon->StopShooting();
		CurrentWeapon->SetActorHiddenInGame(true);
	}

	CurrentWeapon = Weapons[WeaponIndex].Weapon;
	CurrentWeapon->SetActorHiddenInGame(false);
}

void UWeaponComponent::OnEmptyClip(AWeaponBase* TargetWeapon)
{
	if (!TargetWeapon) return;

	if (CurrentWeapon == TargetWeapon)
	{
		Reload();
	}
}

void UWeaponComponent::OnReloadFinished() const
{
	CurrentWeapon->Reload();
	PullAnimationTimeline->ReverseFromEnd();
}

void UWeaponComponent::BroadCastOnWeaponShot()
{
	OnWeaponShot.Broadcast();
}

void UWeaponComponent::CheckIsNearWall()
{
	FVector ViewLocation = FVector::ZeroVector;
	FRotator ViewRotation = FRotator::ZeroRotator;

	if (!FProjectUtils::GetPlayerViewPoint(GetOwner(), ViewLocation, ViewRotation)) return;

	FVector TraceStart = ViewLocation;
	FVector TraceDirection = ViewRotation.Vector();
	FVector TraceEnd = TraceStart + TraceDirection * 125.f;

	if (!GetWorld()) return;

	FCollisionObjectQueryParams CollisionObjectQueryParams;
	CollisionObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());
	FHitResult HitResult;

	GetWorld()->LineTraceSingleByObjectType(HitResult,
	                                        TraceStart,
	                                        TraceEnd,
	                                        CollisionObjectQueryParams,
	                                        CollisionQueryParams);

	bIsNearWall = HitResult.bBlockingHit;

	if (bIsShooting && bIsNearWall)
	{
		StopShooting();
	}
}

void UWeaponComponent::OnPullFinished()
{
	FWeaponData WeaponData;
	CurrentWeapon->GetWeaponData(WeaponData);

	switch (PullCommand)
	{
	case EWeaponPullCommand::Reload:
		if (PullProgress >= 1.f)
		{
			GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle,
			                                       this,
			                                       &UWeaponComponent::OnReloadFinished,
			                                       WeaponData.ReloadTime,
			                                       false);
			return;
		}

		bIsReloading = false;

		if (bIsShooting)
		{
			StartShooting();
		}
		break;

	case EWeaponPullCommand::Equip:
		if (PullProgress >= 1.f)
		{
			EquipWeapon(CurrentWeaponIndex);
			PullAnimationTimeline->ReverseFromEnd();
			return;
		}

		bIsEquipping = false;

		if (bIsShooting)
		{
			StartShooting();
		}
		break;
	}
}

void UWeaponComponent::StartEquipAnimation()
{
	bIsEquipping = true;
	PullCommand = EWeaponPullCommand::Equip;

	if (PullDuration <= 0.f)
	{
		EquipWeapon(CurrentWeaponIndex);
		bIsEquipping = false;
		return;
	}

	PullAnimationTimeline->PlayFromStart();
}

void UWeaponComponent::GetCurrentWeaponAmmo(FWeaponAmmoData& AmmoData) const
{
	AmmoData = CurrentWeapon->GetAmmoData();
}
