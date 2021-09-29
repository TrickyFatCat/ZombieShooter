// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Actors/InteractiveActors/DoorBase.h"
#include "Interfaces/InteractionInterface.h"
#include "DoorSwing.generated.h"

class UInteractionBoxComponent;

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API ADoorSwing : public ADoorBase, public IInteractionInterface
{
	GENERATED_BODY()

public:
	ADoorSwing();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UInteractionBoxComponent* InteractionTrigger = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Door")
	bool bRequireInteraction = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Door", meta=(ClampMint="0"))
	float AutoCloseDelay = 2.f;

	float SwingDirection = 1.f;
	
	float PrevSwingDirection = 1.f;

	UPROPERTY(BlueprintReadOnly)
	FTimerHandle AutoCloseDelayHandle;

	virtual void Disable() override;

	virtual void Enable() override;

	virtual bool ProcessInteraction_Implementation(APlayerController* PlayerController) override;

	virtual void FinishAnimation() override;

	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	                           AActor* OtherActor,
	                           UPrimitiveComponent* OtherComp,
	                           int32 OtherBodyIndex,
	                           bool bFromSweep,
	                           const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent,
	                         AActor* OtherActor,
	                         UPrimitiveComponent* OtherComp,
	                         int32 OtherBodyIndex);

	void CalculateTargetTransform(const AActor* Actor);
};
