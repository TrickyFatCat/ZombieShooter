// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Components/TriggerComponents/BaseSphereTriggerComponent.h"
#include "GameFramework/Actor.h"
#include "PoisonousCloud.generated.h"

UCLASS()
class ZOMBIESHOOTER_API APoisonousCloud : public AActor
{
	GENERATED_BODY()

public:
	APoisonousCloud();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	UBaseSphereTriggerComponent* DamageTrigger = nullptr;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cloud", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UDamageType> DamageType = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cloud", meta=(AllowPrivateAccess="true", ClampMin="0"))
	float Damage = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cloud", meta=(AllowPrivateAccess="true", ClampMin="0"))
	float DamageRate = 5.f;

	FTimerHandle DamageTimerHandle;

	float TimeBetweenTicks = 1.f;

	UPROPERTY()
	AActor* TargetActor = nullptr;

	UFUNCTION()
	virtual void OnDamageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	                                         AActor* OtherActor,
	                                         UPrimitiveComponent* OtherComp,
	                                         int32 OtherBodyIndex,
	                                         bool bFromSweep,
	                                         const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnDamageTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent,
	                                       AActor* OtherActor,
	                                       UPrimitiveComponent* OtherComp,
	                                       int32 OtherBodyIndex);

	UFUNCTION()
	void StartDamageTimer();

	UFUNCTION()
	void StopDamageTimer();

	void DealDamage();
};