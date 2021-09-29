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

	if (GetWorldTimerManager().IsTimerActive(AutoCloseDelayHandle))
	{
		GetWorldTimerManager().ClearTimer(AutoCloseDelayHandle);
	}

	CalculateTargetTransform(PlayerActor);
	StartAnimation();
	return true;
}

void ADoorSwing::FinishAnimation()
{
	Super::FinishAnimation();

	if (AutoCloseDelay <= 0.f || InteractionTrigger->GetIsActorInside()) return;

	GetWorldTimerManager().SetTimer(AutoCloseDelayHandle, this, &ADoorSwing::Close, AutoCloseDelay, false);
}

void ADoorSwing::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                       AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp,
                                       int32 OtherBodyIndex,
                                       bool bFromSweep,
                                       const FHitResult& SweepResult)
{
	CalculateTargetTransform(OtherActor);

	if (AutoCloseDelay > 0.f && GetWorldTimerManager().IsTimerActive(AutoCloseDelayHandle))
	{
		GetWorldTimerManager().ClearTimer(AutoCloseDelayHandle);
		return;
	}

	if (bRequireInteraction || GetStateCurrent() != EInteractiveActorState::Closed) return;

	Open();
}

void ADoorSwing::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                     AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp,
                                     int32 OtherBodyIndex)
{
	if (AutoCloseDelay > 0.f && GetStateCurrent() == EInteractiveActorState::Opened)
	{
		GetWorldTimerManager().SetTimer(AutoCloseDelayHandle, this, &ADoorSwing::Close, AutoCloseDelay, false);
		return;
	}

	if (bRequireInteraction || GetStateCurrent() != EInteractiveActorState::Opened) return;

	Close();
}

void ADoorSwing::CalculateTargetTransform(const AActor* Actor)
{
	if (TargetTransforms.Num() == 0) return;
	
	PrevSwingDirection = SwingDirection;
	const float DotProduct = FVector::DotProduct(GetActorForwardVector(),
	                                             (GetActorLocation() - Actor->GetActorLocation()).GetSafeNormal());
	SwingDirection = FMath::Sign(DotProduct);

	if (PrevSwingDirection != SwingDirection) return;

	const float CurrentYaw = TargetTransforms[0].TargetRotation.Yaw;

	if (SwingDirection < 0.f && FMath::Sign(CurrentYaw) < 0.f)
	{
		TargetTransforms[0].TargetRotation.Yaw *= SwingDirection;
	}
	else if (SwingDirection > 0.f && FMath::Sign(CurrentYaw) > 0.f)
	{
		TargetTransforms[0].TargetRotation.Yaw *= -SwingDirection;
	}
}
