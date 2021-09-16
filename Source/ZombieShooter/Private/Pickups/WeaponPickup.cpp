// Created by Artyom Volkov during the UE4 course


#include "Pickups/WeaponPickup.h"
#include "Components/WeaponComponent.h"

bool AWeaponPickup::ActivatePickup_Implementation(AActor* TargetActor)
{
	if (!IsValid(TargetActor)) return false;

	UWeaponComponent* WeaponComponent =	TargetActor->FindComponentByClass<UWeaponComponent>();

	if (!WeaponComponent) return false;

	WeaponComponent->UnlockWeapon(WeaponClass);
	return true;
}
