// Created by Artyom Volkov during the UE4 course


#include "Components/WeaponComponent.h"

#include "Characters/PlayerCharacter.h"
#include "Components/TimelineComponent.h"
#include "Weapons/WeaponBase.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	PullAnimationTimeline = CreateDefaultSubobject<UTimelineComponent>("PullAnimationTimeline");
}


void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapons();
	EquipWeapon(CurrentWeaponIndex);

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
}

void UWeaponComponent::GetCurrentWeaponData(FWeaponData& WeaponData) const
{
	WeaponData = CurrentWeapon->GetWeaponData();
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
		Weapons.Add(Weapon);
		FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
		Weapon->AttachToComponent(PlayerCharacter->GetPlayerArms(),
		                          AttachmentTransformRules,
		                          Weapon->GetWeaponSocketName());
		Weapon->SetActorHiddenInGame(true);
	}
}

void UWeaponComponent::EquipNextWeapon()
{
	if (bIsEquipping || bIsReloading) return;

	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	StartEquipAnimation();
}

void UWeaponComponent::EquipPreviousWeapon()
{
	if (bIsEquipping || bIsReloading) return;

	CurrentWeaponIndex = CurrentWeaponIndex == 0 ? Weapons.Num() - 1 : CurrentWeaponIndex - 1;
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
	if (!CurrentWeapon && bIsEquipping && bIsReloading && !CurrentWeapon->CanReload()) return;

	CurrentWeapon->StopShooting();
	bIsReloading = true;
	PullCommand = EWeaponPullCommand::Reload;

	if (CurrentWeapon->GetWeaponData().ReloadTime <= 0.f)
	{
		CurrentWeapon->Reload();
		bIsReloading = false;
		return;
	}

	PullAnimationTimeline->SetTimelineLength(CurrentWeapon->GetWeaponData().ReloadTime * 0.5);
	PullAnimationTimeline->PlayFromStart();
}


void UWeaponComponent::EquipWeapon(const int32 WeaponIndex)
{
	if (!GetOwner() || WeaponIndex >= Weapons.Num() || Weapons.Num() == 0) return;

	if (CurrentWeapon)
	{
		CurrentWeapon->StopShooting();
		CurrentWeapon->SetActorHiddenInGame(true);
	}

	CurrentWeapon = Weapons[WeaponIndex];
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

void UWeaponComponent::OnPullFinished()
{
	switch (PullCommand)
	{
	case EWeaponPullCommand::Reload:
		if (PullProgress >= 1.f)
		{
			CurrentWeapon->Reload();
			PullAnimationTimeline->ReverseFromEnd();
			return;
		}

		bIsReloading = false;
		break;

	case EWeaponPullCommand::Equip:
		if (PullProgress >= 1.f)
		{
			EquipWeapon(CurrentWeaponIndex);
			PullAnimationTimeline->ReverseFromEnd();
			return;
		}

		bIsEquipping = false;
		break;
	}
}

void UWeaponComponent::StartEquipAnimation()
{
	bIsEquipping = true;
	PullCommand = EWeaponPullCommand::Equip;

	if (EquipDuration <= 0.f)
	{
		EquipWeapon(CurrentWeaponIndex);
		bIsEquipping = false;
		return;
	}

	PullAnimationTimeline->SetPlayRate(1.f / (EquipDuration * 0.5f));
	PullAnimationTimeline->PlayFromStart();
}

void UWeaponComponent::GetCurrentWeaponAmmo(FWeaponAmmoData& AmmoData) const
{
	AmmoData = CurrentWeapon->GetAmmoData();
}
