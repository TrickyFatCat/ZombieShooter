// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Components/TriggerComponents/BaseCapsuleTriggerComponent.h"
#include "DamageCapsuleComponent.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API UDamageCapsuleComponent : public UBaseCapsuleTriggerComponent
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
public:
	void SetDamage(const int32 Value);
	
	void SetDamageTypeClass(const TSubclassOf<UDamageType> Value) { DamageTypeClass = Value; };
	
protected:
	int32 Damage = 10;

	TSubclassOf<UDamageType> DamageTypeClass = nullptr;

	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	                            AActor* OtherActor,
	                            UPrimitiveComponent* OtherComp,
	                            int32 OtherBodyIndex,
	                            bool bFromSweep,
	                            const FHitResult& SweepResult) override;
};
