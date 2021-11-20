// Created by Artyom Volkov during the UE4 course


#include "Pickups/PickupArmor.h"
#include "Components/ShooterDamageControllerComponent.h"

bool APickupArmor::ActivatePickup_Implementation(AActor* TargetActor)
{
	if (!TargetActor) return false;

	UShooterDamageControllerComponent* DamageController = TargetActor->FindComponentByClass<
		UShooterDamageControllerComponent>();

	if (!DamageController) return false;

	if (DamageController->GetNormalizedArmor() >= 1.f || DamageController->GetIsDead()) return false;

	DamageController->IncreaseArmor(ArmorAmount, bClampToMax);
	
	return true;
}
