// Created by Artyom Volkov during the UE4 course


#pragma once

#include "CoreMinimal.h"
#include "Components/WeaponComponent.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UShooterDamageControllerComponent;
class AWeaponBase;
class UFootstepsAudioComponent;

UCLASS()
class ZOMBIESHOOTER_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components")
	UShooterDamageControllerComponent* DamageControllerComponent = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components")
	UWeaponComponent* WeaponComponent = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components")
	UFootstepsAudioComponent* FootstepsAudioComponent = nullptr;

	UFUNCTION()
	virtual void OnDeath(AController* DeathInstigator, AActor* DeathCauser, const UDamageType* DamageType);

public:
	UFUNCTION(BlueprintCallable, Category="Weapon")
	AWeaponBase* GetCurrentWeapon() const { return WeaponComponent->GetCurrentWeapon(); }
};
