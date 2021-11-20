// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Actors/PickupBase.h"
#include "PickupAmmo.generated.h"

class AWeaponBase;

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API APickupAmmo : public APickupBase
{
	GENERATED_BODY()

public:
	APickupAmmo();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pickup", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* AmmoCrateMesh = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pickup", meta=(AllowPrivateAccess="true"))
	TSubclassOf<AWeaponBase> WeaponClass = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pickup", meta=(AllowPrivateAccess="true"))
	int32 AmmoAmount = 10;

	virtual bool ActivatePickup_Implementation(AActor* TargetActor) override;
};
