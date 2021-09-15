// Created by Artyom Volkov during the UE4 course


#include "Pickups/WeaponPickup.h"
#include "Components/WeaponComponent.h"

void AWeaponPickup::ActivatePickup_Implementation(AActor* TargetActor)
{
	if (!IsValid(TargetActor)) return;

	UWeaponComponent* WeaponComponent =	TargetActor->FindComponentByClass<UWeaponComponent>();

	if (!WeaponComponent) return;

	WeaponComponent->UnlockWeapon(WeaponClass);
}
