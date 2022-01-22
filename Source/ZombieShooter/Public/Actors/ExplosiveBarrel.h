// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplosiveBarrel.generated.h"

class UDamageType;
class UStaticMeshComponent;
class UWeaponFXComponent;
class UCapsuleComponent;

UCLASS()
class ZOMBIESHOOTER_API AExplosiveBarrel : public AActor
{
	GENERATED_BODY()

public:
	AExplosiveBarrel();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* BarrelMesh = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UWeaponFXComponent* ExplosionFXComponent = nullptr;

	UPROPERTY(EditDefaultsOnly,
		BlueprintReadOnly,
		Category="ExplosiveBarrel",
		meta=(AllowPrivateAccess="true", ClampMin="0"))
	int32 ExplosionDamage = 100.f;

	UPROPERTY(EditDefaultsOnly,
		BlueprintReadOnly,
		Category="ExplosiveBarrel",
		meta=(AllowPrivateAccess="true", ClampMin="0"))
	float ExplosionRadius = 200.f;

	UPROPERTY(EditDefaultsOnly,
		BlueprintReadOnly,
		Category="ExplosiveBarrel",
		meta=(AllowPrivateAccess="true", ClampMin="0"))
	bool bDoFullDamage = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ExplosiveBarrel", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UDamageType> DamageTypeClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ExplosiveBarrel", meta=(AllowPrivateAccess="true", ClampMin="0"))
	float ExplosionDelay = 0.2f;

	bool bIsExploded = false;

	FTimerHandle ExplosionDelayHandle;

	UFUNCTION()
	virtual void OnTakeDamage(AActor* DamagedActor,
	                          float Damage,
	                          const UDamageType* DamageType,
	                          AController* InstigatedBy,
	                          AActor* DamageCauser);

	UFUNCTION()
	void ProcessExplosion();
};
