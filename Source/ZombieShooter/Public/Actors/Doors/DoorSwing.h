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
class ZOMBIESHOOTER_API ADoorSwing : public ADoorBase
{
	GENERATED_BODY()

public:
	ADoorSwing();

protected:
	virtual void BeginPlay() override;

protected:
	float SwingDirection = 1.f;

	float PrevSwingDirection = 1.f;

	virtual bool ProcessInteraction_Implementation(APlayerController* PlayerController) override;

	virtual void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	                                   AActor* OtherActor,
	                                   UPrimitiveComponent* OtherComp,
	                                   int32 OtherBodyIndex,
	                                   bool bFromSweep,
	                                   const FHitResult& SweepResult) override;


	void CalculateTargetTransform(const AActor* Actor);

	virtual void ProcessAutoClose() override;
};
