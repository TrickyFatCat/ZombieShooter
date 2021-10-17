// Created by Artyom Volkov during the UE4 course


#include "Actors/Doors/DoorSwing.h"
#include "Components/TriggerComponents/InteractionBoxComponent.h"

ADoorSwing::ADoorSwing()
{
}


void ADoorSwing::BeginPlay()
{
	Super::BeginPlay();
}

bool ADoorSwing::ProcessInteraction_Implementation(APlayerController* PlayerController)
{
	const AActor* PlayerActor = Cast<AActor>(PlayerController->GetPawn());

	if (!PlayerActor) return false;

	CalculateTargetTransform(PlayerActor);
	return Super::ProcessInteraction_Implementation(PlayerController);
}


void ADoorSwing::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                       AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp,
                                       int32 OtherBodyIndex,
                                       bool bFromSweep,
                                       const FHitResult& SweepResult)
{
	if (GetDoorType() == EDoorType::Interactive)
	{
		CalculateTargetTransform(OtherActor);
	}

	Super::OnTriggerBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ADoorSwing::CalculateTargetTransform(const AActor* Actor)
{
	if (TargetTransforms.Num() == 0 || !Actor) return;

	PrevSwingDirection = SwingDirection;
	const float DotProduct = FVector::DotProduct(GetActorForwardVector(),
	                                             (GetActorLocation() - Actor->GetActorLocation()).GetSafeNormal());
	SwingDirection = FMath::Sign(DotProduct);

	const float CurrentYaw = TargetTransforms[0].TargetRotation.Yaw;

	if (PrevSwingDirection != SwingDirection)
	{
		PrevSwingDirection = SwingDirection;;
		return;
	}

	const float CurrentYawSign = FMath::Sign(CurrentYaw);

	if (SwingDirection < 0.f && CurrentYawSign < 0.f)
	{
		TargetTransforms[0].TargetRotation.Yaw *= SwingDirection;
	}
	else if (SwingDirection > 0.f && CurrentYawSign > 0.f)
	{
		TargetTransforms[0].TargetRotation.Yaw *= -SwingDirection;
	}
}

void ADoorSwing::ProcessAutoClose()
{
	SwingDirection *= -1;
	Close();
}
