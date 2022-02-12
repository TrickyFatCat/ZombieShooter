// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Actors/PickupBase.h"
#include "PickupWeapon.generated.h"

class AWeaponBase;

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API APickupWeapon : public APickupBase
{
	GENERATED_BODY()
public:
	APickupWeapon();
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Component", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* WeaponMesh = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pickup", meta=(AllowPrivateAccess="true"))
	TSubclassOf<AWeaponBase> WeaponClass;
	
	virtual bool ActivatePickupEffect_Implementation(AActor* TargetActor) override;
};
