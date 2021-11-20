// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Characters/PlayerCharacter.h"
#include "Components/SceneComponent.h"
#include "PlayerArmsComponent.generated.h"

class UCharacterMovementComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ZOMBIESHOOTER_API UPlayerArmsComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UPlayerArmsComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime,
	                           ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	FRotator InitialRotation = FRotator::ZeroRotator;
	
	FVector InitialLocation = FVector::ZeroVector;

	UPROPERTY()
	UCharacterMovementComponent* PlayerMovementComponent = nullptr;
	// Sway
public:
	void SetHorizontalSway(const float AxisValue);

	void SetVerticalSway(const float AxisValue);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PlayerArms|Sway", meta=(AllowPrivateAccess="true"))
	float SwayPower = 2.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PlayerArms|Sway", meta=(AllowPrivateAccess="true"))
	float SwaySpeed = 100.f;

	void ProcessSwayRotation(const float DeltaTime);

	// Weapon Offset
public:
	void StartShooting() { bIsShooting = true; }

	void StopShooting() { bIsShooting = false; }
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PlayerArms", meta=(AllowPrivateAccess="true"))
	float LocationOffset = 2.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PlayerArms", meta=(AllowPrivateAccess="true", ClampMin="0", ClampMax="0"))
	float RecoverLerpAlpha = 0.1f;

	UPROPERTY()
	APlayerCharacter* PlayerCharacter = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PlayerArms|MovementAnimation", meta=(AllowPrivateAccess="true"))
	float AnimationFrequency = 8.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PlayerArms|MovementAnimation", meta=(AllowPrivateAccess="true"))
	FVector MovementOffset = FVector(0.f, 1.f, 0.5f);

	bool bIsShooting = false;
};
