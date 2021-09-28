// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplosiveBarrel.generated.h"

class UDamageType;
class UStaticMeshComponent;
class UWeaponFXComponent;

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* BarrelMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UWeaponFXComponent* ExplosionFXComponent = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ExplosiveBarrel", meta=(AllowPrivateAccess="true", ClampMin="0"))
	int32 ExplosionDamage = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ExplosiveBarrel", meta=(AllowPrivateAccess="true", ClampMin="0"))
	float ExplosionRadius = 200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ExplosiveBarrel", meta=(AllowPrivateAccess="true", ClampMin="0"))
	bool bDoFullDamage = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ExplosiveBarrel", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UDamageType> DamageTypeClass = nullptr;

	bool bIsExploded = false;

	

	UFUNCTION()
	virtual void OnTakeDamage(AActor* DamagedActor,
	                             float Damage,
	                             const UDamageType* DamageType,
	                             AController* InstigatedBy,
	                             AActor* DamageCauser);
	UFUNCTION()
	virtual void OnRadialDamage(AActor* DamagedActor,
	                                float Damage,
	                                const class UDamageType* DamageType,
	                                FVector Origin,
	                                FHitResult HitInfo,
	                                class AController* InstigatedBy,
	                                AActor* DamageCauser);
};
