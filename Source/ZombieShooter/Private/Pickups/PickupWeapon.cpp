// Created by Artyom Volkov during the UE4 course


#include "Pickups/PickupWeapon.h"
#include "Components/WeaponComponent.h"
#include "Components/SkeletalMeshComponent.h"

APickupWeapon::APickupWeapon()
{
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");
	WeaponMesh->SetupAttachment(MeshScene);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

bool APickupWeapon::ActivatePickup_Implementation(AActor* TargetActor)
{
	if (!IsValid(TargetActor)) return false;

	UWeaponComponent* WeaponComponent =	TargetActor->FindComponentByClass<UWeaponComponent>();

	if (!WeaponComponent) return false;

	return WeaponComponent->UnlockWeapon(WeaponClass);
}
