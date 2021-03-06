//  Created by Artyom Volkov during the UE4 course


#include "Characters/BaseCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/FootstepsAudioComponent.h"
#include "Components/ShooterDamageControllerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	DamageControllerComponent = CreateDefaultSubobject<UShooterDamageControllerComponent>("DamageController");
	
	FootstepsAudioComponent = CreateDefaultSubobject<UFootstepsAudioComponent>("FootstepsAudio");
	FootstepsAudioComponent->SetupAttachment(GetRootComponent());
	FootstepsAudioComponent->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	DamageControllerComponent->OnDeath.AddDynamic(this, &ABaseCharacter::OnDeath);
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::OnDeath(AController* DeathInstigator, AActor* DeathCauser, const UDamageType* DamageType)
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetCharacterMovement()->DisableMovement();
}
