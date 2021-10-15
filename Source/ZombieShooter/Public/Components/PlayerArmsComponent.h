// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PlayerArms", meta=(AllowPrivateAccess="true"))
	float SwayPower = 2.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PlayerArms", meta=(AllowPrivateAccess="true"))
	float SwaySpeed = 100.f;

	void ProcessSwayRotation(const float DeltaTime);
	
	// Jump offset
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PlayerArms", meta=(AllowPrivateAccess="true"))
	FVector JumpLocationOffset = FVector(0.f, 0.f, -4.f);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PlayerArms", meta=(AllowPrivateAccess="true", ClampMin="0", ClampMax="0"))
	float JumpLerpSpeed = 0.05f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PlayerArms", meta=(AllowPrivateAccess="true", ClampMin="0", ClampMax="0"))
	float RecoverLerpSpeed = 0.1f;

	void SetJumpOffsetLocation();

	void RecoverLocation();
};
