// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapons/WeaponCoreTypes.h"
#include "WeaponFXComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBIESHOOTER_API UWeaponFXComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWeaponFXComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PlayImpactFX(const FHitResult& HitResult);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="VFX")
	FHitScanImpactData DefaultImpactData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="VFX")
	TMap<TEnumAsByte<EPhysicalSurface>, FHitScanImpactData> ImpactDataMap;
};
