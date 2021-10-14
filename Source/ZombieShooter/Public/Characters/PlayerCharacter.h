// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class UWeaponComponent;
class UInteractionQueueComponent;

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

	USceneComponent* GetPlayerArms() const { return PlayerArms; }

	UFUNCTION(BlueprintPure, Category="Player")
	float GetAdsTransitionProgress() const { return WeaponComponent->GetAdsTransitionProgress(); }

	UFUNCTION(BlueprintPure, Category="Player")
	bool GetIsUsingScope() const { return WeaponComponent->GetIsUsingScope(); }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	UCameraComponent* PlayerCamera = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	USceneComponent* PlayerArms = nullptr;

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

	// WeaponSway
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation", meta=(AllowPrivateAccess="true"))
	float SwayPower = 2.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation", meta=(AllowPrivateAccess="true"))
	float SwaySpeed = 100.f;

	UPROPERTY(VisibleInstanceOnly)
	FRotator InitialRotation = FRotator::ZeroRotator;
	
	void SetHorizontalSway(const float AxisValue);

	void SetVerticalSway(const float AxisValue);

	void ProcessSwayRotation(const float DeltaTime) const;

	// Camera recoil
public:
	UFUNCTION()
	void AddCameraRecoil(const float RecoilPitch, const float RecoilYaw);

	// Interaction
private:
	void StartInteraction();
};