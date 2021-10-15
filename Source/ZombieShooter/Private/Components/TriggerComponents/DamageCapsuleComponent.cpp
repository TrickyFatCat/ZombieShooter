// Created by Artyom Volkov during the UE4 course


#include "Components/TriggerComponents/DamageCapsuleComponent.h"

#include "Kismet/GameplayStatics.h"

void UDamageCapsuleComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDamageCapsuleComponent::SetDamage(const int32 Value)
{
	if (Value < 0) return;

	Damage = Value;
}

void UDamageCapsuleComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                             AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp,
                                             int32 OtherBodyIndex,
                                             bool bFromSweep,
                                             const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	AController* OwnerController = nullptr;

	if (OwnerPawn)
	{
		OwnerController = OwnerPawn->GetController();
	}

	UGameplayStatics::ApplyPointDamage(OtherActor,
	                                   Damage,
	                                   GetOwner()->GetActorLocation(),
	                                   SweepResult,
	                                   OwnerController,
	                                   GetOwner(),
	                                   DamageTypeClass);
}
