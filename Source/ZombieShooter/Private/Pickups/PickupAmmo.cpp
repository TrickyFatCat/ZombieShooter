// Created by Artyom Volkov during the UE4 course


#include "Pickups/PickupAmmo.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WeaponComponent.h"

APickupAmmo::APickupAmmo()
{
	AmmoCrateMesh = CreateDefaultSubobject<UStaticMeshComponent>("AmmoCrateMesh");
	AmmoCrateMesh->SetupAttachment(MeshScene);
}

bool APickupAmmo::ActivatePickup_Implementation(AActor* TargetActor)
{
	if (!IsValid(TargetActor)) return false;

	UWeaponComponent* WeaponComponent = TargetActor->FindComponentByClass<UWeaponComponent>();

	if (!WeaponComponent) return false;

	return WeaponComponent->RestoreStorageAmmo(WeaponClass, AmmoAmount);
}
