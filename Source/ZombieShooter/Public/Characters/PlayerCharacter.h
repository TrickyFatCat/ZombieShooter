// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class UWeaponComponent;
class UInteractionQueueComponent;
class UPlayerArmsComponent;

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPlayerArmsComponent* GetPlayerArms() const { return PlayerArms; }

	UFUNCTION(BlueprintPure, Category="Player")
	float GetAdsTransitionProgress() const { return WeaponComponent->GetAdsTransitionProgress(); }

	UFUNCTION(BlueprintPure, Category="Player")
	bool GetIsUsingScope() const { return WeaponComponent->GetIsUsingScope(); }

	bool CanShoot() const { return WeaponComponent->CanShoot(); }

	bool IsShooting() const { return WeaponComponent->GetIsShooting(); }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	UCameraComponent* PlayerCamera = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	UPlayerArmsComponent* PlayerArms = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	UInteractionQueueComponent* InteractionQueue = nullptr;

	virtual void OnDeath(AController* DeathInstigator, AActor* DeathCauser, const UDamageType* DamageType) override;
	
	// Movement
public:
protected:
	float DefaultMaxSpeed = 750.f;

	float DefaultInputYawScale = 1.f;

	float DefaultInputPitchScale = 1.f;
	
	void MoveForward(const float AxisValue);
	
	void MoveRight(const float AxisValue);

	UFUNCTION()
	void OnEnterAds();

	UFUNCTION()
	void OnExitAds();

	UFUNCTION()
	void EnterCrouch();

	UFUNCTION()
	void ExitCrouch();

	// Camera recoil
public:
	UFUNCTION()
	void AddCameraRecoil(const float RecoilPitch, const float RecoilYaw);

	// Interaction
private:
	void StartInteraction();
};