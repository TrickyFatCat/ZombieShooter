// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Actors/PickupBase.h"
#include "WeaponPickup.generated.h"

class AWeaponBase;

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API AWeaponPickup : public APickupBase
{
	GENERATED_BODY()
public:
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pickup", meta=(AllowPrivateAccess="true"))
	TSubclassOf<AWeaponBase> WeaponClass;

	virtual bool ActivatePickup_Implementation(AActor* TargetActor) override;
};
