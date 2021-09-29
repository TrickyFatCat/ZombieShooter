// Created by Artyom Volkov during the UE4 course


#include "Actors/Doors/DoorSwing.h"
#include "Components/TriggerComponents/InteractionBoxComponent.h"

ADoorSwing::ADoorSwing()
{
	InteractionTrigger = CreateDefaultSubobject<UInteractionBoxComponent>("InteractionTrigger");
	SetRootComponent(InteractionTrigger);
}


void ADoorSwing::BeginPlay()
{
	Super::BeginPlay();

	InteractionTrigger->SetIsNormalTrigger(!bRequireInteraction);
	InteractionTrigger->bRequireLineOfSight = bRequireInteraction;

	InteractionTrigger->OnComponentBeginOverlap.AddDynamic(this, &ADoorSwing::OnTriggerBeginOverlap);
	InteractionTrigger->OnComponentEndOverlap.AddDynamic(this, &ADoorSwing::OnTriggerEndOverlap);
}

void ADoorSwing::Disable()
{
	Super::Disable();

	InteractionTrigger->SetIsEnabled(false);
}

void ADoorSwing::Enable()
{
	Super::Enable();

	InteractionTrigger->SetIsEnabled(true);
}

bool ADoorSwing::ProcessInteraction_Implementation(APlayerController* PlayerController)
{
	if (!PlayerController || !bRequireInteraction) return false;

	if (!GetIsReversible() && GetStateCurrent() == EInteractiveActorState::Transition) return false;

	const AActor* PlayerActor = Cast<AActor>(PlayerController->GetPawn());

	if (!PlayerActor) return false;

	StopAutoClose();
	CalculateTargetTransform(PlayerActor);
	StartAnimation();
	return true;
}

void ADoorSwing::FinishAnimation()
{
	Super::FinishAnimation();

	if (AutoCloseDelay <= 0.f || InteractionTrigger->GetIsActorInside()) return;

	StartAutoClose();
}

void ADoorSwing::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                       AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp,
                                       int32 OtherBodyIndex,
                                       bool bFromSweep,
                                       const FHitResult& SweepResult)
{
	if (AutoCloseDelay > 0.f)
	{
		StopAutoClose();
		return;
	}

	if (bRequireInteraction || GetStateCurrent() != EInteractiveActorState::Closed) return;

	CalculateTargetTransform(OtherActor);
	Open();
}

void ADoorSwing::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                     AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp,
                                     int32 OtherBodyIndex)
{
	if (AutoCloseDelay > 0.f && IsStateCurrent(EInteractiveActorState::Opened))
	{
		StartAutoClose();
		return;
	}

	if (bRequireInteraction || GetStateCurrent() != EInteractiveActorState::Opened) return;

	Close();
}

void ADoorSwing::CalculateTargetTransform(const AActor* Actor)
{
	if (TargetTransforms.Num() == 0 || !Actor) return;

	PrevSwingDirection = SwingDirection;
	const float DotProduct = FVector::DotProduct(GetActorForwardVector(),
	                                             (GetActorLocation() - Actor->GetActorLocation()).GetSafeNormal());
	SwingDirection = FMath::Sign(DotProduct);

	const float CurrentYaw = TargetTransforms[0].TargetRotation.Yaw;

	UE_LOG(LogTemp, Error, TEXT("Previous Direction: %f | Current Dircetion; %f"), PrevSwingDirection, SwingDirection);

	if (PrevSwingDirection != SwingDirection)
	{
		PrevSwingDirection = SwingDirection;;
		UE_LOG(LogTemp, Error, TEXT("Target Yaw_1: %f"), TargetTransforms[0].TargetRotation.Yaw);
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

	UE_LOG(LogTemp, Error, TEXT("Target Yaw: %f"), TargetTransforms[0].TargetRotation.Yaw);
}

void ADoorSwing::StartAutoClose()
{
	GetWorldTimerManager().SetTimer(AutoCloseDelayHandle, this, &ADoorSwing::ProcessAutoClose, AutoCloseDelay, false);
}

void ADoorSwing::StopAutoClose()
{
	if (GetWorldTimerManager().IsTimerActive(AutoCloseDelayHandle))
	{
		GetWorldTimerManager().ClearTimer(AutoCloseDelayHandle);
	}
}

void ADoorSwing::ProcessAutoClose()
{
	SwingDirection *= -1;
	Close();
}
