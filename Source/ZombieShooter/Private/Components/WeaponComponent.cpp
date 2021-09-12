// Created by Artyom Volkov during the UE4 course


#include "Components/WeaponComponent.h"

#include "Characters/PlayerCharacter.h"
#include "Weapons/WeaponBase.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapons();
	EquipWeapon(CurrentWeaponIndex);
}


void UWeaponComponent::TickComponent(float DeltaTime,
                                     ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
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

		// Add delegate for auto reload
		Weapon->SetOwner(GetOwner());
		Weapons.Add(Weapon);
		FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
		Weapon->AttachToComponent(PlayerCharacter->GetPlayerArms(), AttachmentTransformRules, Weapon->GetWeaponSocketName());
		Weapon->SetActorHiddenInGame(true);
	}
}

void UWeaponComponent::EquipNextWeapon()
{
	if (bIsEquipping && bIsReloading) return;

	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	bIsEquipping = true;
	EquipWeapon(CurrentWeaponIndex);
	bIsEquipping = false;
	// Play equip animation
}

void UWeaponComponent::EquipPreviousWeapon()
{
	if (bIsEquipping && bIsReloading) return;

	CurrentWeaponIndex = CurrentWeaponIndex == 0 ? Weapons.Num() - 1 : CurrentWeaponIndex - 1;
	bIsEquipping = true;
	EquipWeapon(CurrentWeaponIndex);
	bIsEquipping = false;
	// Play equip animation
}

void UWeaponComponent::StartShooting()
{
}

void UWeaponComponent::StopShooting()
{
}

void UWeaponComponent::Reload()
{
}

void UWeaponComponent::EquipWeapon(const int32 WeaponIndex)
{
	if (!GetOwner() || WeaponIndex >= Weapons.Num() || Weapons.Num() == 0) return;

	if (CurrentWeapon)
	{
		// CurrentWeapon->StopShooting();
		CurrentWeapon->SetActorHiddenInGame(true);
	}

	CurrentWeapon = Weapons[WeaponIndex];
	CurrentWeapon->SetActorHiddenInGame(false);
}