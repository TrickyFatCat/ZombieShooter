// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;

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

	USkeletalMeshComponent* GetPlayerArms() const { return PlayerArms; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	UCameraComponent* PlayerCamera = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	USkeletalMeshComponent* PlayerArms = nullptr;
	// Movement
public:
protected:
	void MoveForward(const float AxisValue);
	void MoveRight(const float AxisValue);

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
};
