// Created by Artyom Volkov during the UE4 course


#include "Components/ShooterDamageControllerComponent.h"

#include "Perception/AISense_Damage.h"

void UShooterDamageControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	ArmorObject = NewObject<UEntityResource>(this, TEXT("ArmorObject"));
	ArmorObject->SetResourceData(ArmorData);
	ArmorObject->OnValueChanged.AddDynamic(this, &UShooterDamageControllerComponent::BroadcastOnArmorChanged);
}

void UShooterDamageControllerComponent::DecreaseArmor(const float Amount, AController* Instigator)
{
	if (Amount <= 0.f || GetIsDead()) return;

	ArmorObject->DecreaseValue(Amount);

	if (GetIsDead())
	{
		ArmorObject->SetAutoIncreaseEnabled(false);
	}
}

void UShooterDamageControllerComponent::IncreaseArmor(const float Amount, const bool bClampToMax)
{
	if (Amount <= 0.f || GetIsDead()) return;

	ArmorObject->IncreaseValue(Amount, bClampToMax);
}

void UShooterDamageControllerComponent::DecreaseMaxArmor(const float Amount, const bool bClampCurrentArmor)
{
	if (Amount <= 0.f) return;

	ArmorObject->DecreaseValueMax(Amount, bClampCurrentArmor);
}

void UShooterDamageControllerComponent::IncreaseMaxArmor(const float Amount, const bool bClampCurrentArmor)
{
	if (Amount <= 0.f) return;

	ArmorObject->IncreaseValueMax(Amount, bClampCurrentArmor);
}

void UShooterDamageControllerComponent::BroadcastOnArmorChanged(const float NewArmor, const float DeltaArmor)
{
	OnArmorChanged.Broadcast(NewArmor, DeltaArmor);
}

void UShooterDamageControllerComponent::SetArmorModifier(const float Value)
{
	if (Value < 0.f) return;

	ArmorModifier = Value;
}

void UShooterDamageControllerComponent::CalculateDamage_Implementation(const float Damage,
                                                                       AActor* DamagedActor,
                                                                       AController* Instigator,
                                                                       AActor* Causer,
                                                                       const UDamageType* DamageType)
{
	if (Damage <= 0.f) return;

	float CurrentDamage = Damage * GeneralDamageModifier;

	if (GetArmor() > 0.f)
	{
		if (GetArmor() < CurrentDamage * ArmorModifier)
		{
			CurrentDamage -= GetArmor();
			DecreaseArmor(GetArmor(), Instigator);
			DecreaseHealth(CurrentDamage);
		}
		else
		{
			DecreaseArmor(FMath::CeilToFloat(CurrentDamage * ArmorModifier), Instigator);
		}
	}
	else
	{
		Super::CalculateDamage(CurrentDamage, DamagedActor, Instigator, Causer, DamageType);
	}

	if (GetIsDead()) return;

	ReportDamageEvent(CurrentDamage, Instigator, Causer);
}

void UShooterDamageControllerComponent::ReportDamageEvent(const float Damage,
                                                          const AController* Instigator,
                                                          const AActor* Causer) const
{
	AActor* InstigatorPawn = Instigator == nullptr ? nullptr : Instigator->GetPawn();
	const FVector EventLocation = Instigator == nullptr
		                              ? Causer->GetActorLocation()
		                              : InstigatorPawn->GetActorLocation();
	UAISense_Damage::ReportDamageEvent(GetWorld(),
	                                   GetOwner(),
	                                   InstigatorPawn,
	                                   Damage,
	                                   EventLocation,
	                                   GetOwner()->GetActorLocation());
}
