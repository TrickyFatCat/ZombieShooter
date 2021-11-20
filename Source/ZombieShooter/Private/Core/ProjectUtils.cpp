// Created by Artyom Volkov during the UE4 course

#pragma once

#include "Core/ProjectUtils.h"

#include "Components/DamageControllerComponent.h"
#include "Components/ShooterDamageControllerComponent.h"
#include "GameFramework/Character.h"

bool FProjectUtils::GetPlayerViewPoint(AActor* CharacterActor, FVector& ViewLocation, FRotator& ViewRotation)
{
	ACharacter* Character = Cast<ACharacter>(CharacterActor);

	if (!Character) return false;

	if (Character->IsPlayerControlled())
	{
		const APlayerController* Controller = Character->GetController<APlayerController>();

		if (!Controller) return false;

		Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	}

	return true;
}

bool FProjectUtils::GetIsActorDead(const AActor* TargetActor)
{
	UShooterDamageControllerComponent* DamageControllerComponent = TargetActor->FindComponentByClass<UShooterDamageControllerComponent>();

	if (!DamageControllerComponent) return true;

	return DamageControllerComponent->GetIsDead();
}
