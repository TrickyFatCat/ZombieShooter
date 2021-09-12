// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapons/WeaponCoreTypes.h"
#include "WeaponComponent.generated.h"

class AWeaponBase;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ZOMBIESHOOTER_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime,
	                           ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintGetter, Category="Weapon")
	AWeaponBase* GetCurrentWeapon() const { return CurrentWeapon; }

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapons", meta=(AllowPrivateAccess="true"))
	TArray<TSubclassOf<AWeaponBase>> WeaponClasses;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapons", meta=(AllowPrivateAccess="true"))
	TArray<AWeaponBase*> Weapons;
	
	UPROPERTY(BlueprintGetter=GetCurrentWeapon)
	AWeaponBase* CurrentWeapon = nullptr;

	int32 CurrentWeaponIndex = 0;

	void SpawnWeapons();

	// Weapon control
public:
	void EquipNextWeapon();
	void EquipPreviousWeapon();
	void StartShooting();
	void StopShooting();
	void Reload();
protected:
	void EquipWeapon(const int32 WeaponIndex);

	// Weapon parameters
public:
	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool CanFire() const { return !bIsReloading && !bIsEquipping; }
	
	UFUNCTION(BlueprintGetter, Category="Weapon")
	bool GetIsReloading() const { return bIsReloading; }

	UFUNCTION(BlueprintGetter, Category="Weapon")
	bool GetIsEquipping() const { return bIsEquipping; }

protected:
	
private:
	UPROPERTY(BlueprintGetter=GetIsReloading, Category="Weapon")
	bool bIsReloading = false;

	UPROPERTY(BlueprintGetter=GetIsEquipping, Category="Weapon")
	bool bIsEquipping = false;
};
