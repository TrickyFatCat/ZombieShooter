// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "WeaponCoreTypes.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class USkeletalMeshComponent;

UCLASS()
class ZOMBIESHOOTER_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AWeaponBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	EWeaponType GetWeaponType() const { return WeaponTypeData.WeaponType; }

	UFUNCTION(BlueprintCallable, Category="Weapon")
	FName GetWeaponSocketName() const { return WeaponTypeData.WeaponSocketName; }

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess="true"))
	USceneComponent* WeaponRoot = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess="true"))
	USkeletalMeshComponent* WeaponMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon", meta=(AllowPrivateAccess="true"))
	FWeaponTypeData WeaponTypeData{EWeaponType::Pistol, "WeaponPoint"};
};
