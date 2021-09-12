//  Created by Artyom Volkov during the UE4 course


#include "Characters/BaseCharacter.h"
#include "Components/ShooterDamageControllerComponent.h"
#include "Components/WeaponComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	DamageControllerComponent = CreateDefaultSubobject<UShooterDamageControllerComponent>("DamageController");
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("WeaponComponent");
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}