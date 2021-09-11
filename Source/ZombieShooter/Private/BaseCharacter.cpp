//  Created by Artyom Volkov during the UE4 course


#include "BaseCharacter.h"
#include "Components/ShooterDamageControllerComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	DamageControllerComponent = CreateDefaultSubobject<UShooterDamageControllerComponent>("DamageController");
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

