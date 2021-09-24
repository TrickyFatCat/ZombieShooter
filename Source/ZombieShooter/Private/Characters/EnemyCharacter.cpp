// Created by Artyom Volkov during the UE4 course


#include "Characters/EnemyCharacter.h"

#include "BrainComponent.h"
#include "Characters/Controllers/ZombieAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = AZombieAIController::StaticClass();
	bUseControllerRotationYaw = false;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 200.f, 0.f);
	}
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyCharacter::OnDeath(AController* DeathInstigator, AActor* DeathCauser, const UDamageType* DamageType)
{
	Super::OnDeath(DeathInstigator, DeathCauser, DamageType);

	if (BaseExplosionClass && DamageType->IsA(BaseExplosionClass))
	{
		GetMesh()->SetSimulatePhysics(true);
		return;
	}

	const float DotProduct = FVector::DotProduct(GetActorForwardVector(),
	                                             DeathInstigator->GetPawn()->GetActorForwardVector());
	const FName StartSectionName = DotProduct > 0.f ? DeathAnimMontageSections[0] : DeathAnimMontageSections[1];
	PlayAnimMontage(DeathAnimMontage, 1.f, StartSectionName);
	SetLifeSpan(DefaultLifeSpan);

	AAIController* AIController = Cast<AAIController>(Controller);

	if (AIController && AIController->BrainComponent)
	{
		AIController->BrainComponent->Cleanup();
	}
}

bool AEnemyCharacter::GetIsRunning() const
{
	return GetCharacterMovement()->MaxWalkSpeed > MovementData.RunningSpedDefault + MovementData.
		RunningSpeedRandomDeviation;
}

void AEnemyCharacter::SetIsRunning(const bool bIsRunning) const
{
	float NewSpeed = 0.f;
	float DeltaSpeed = 0.f;

	if (bIsRunning)
	{
		DeltaSpeed = FMath::FRandRange(-MovementData.RunningSpeedRandomDeviation,
		                               MovementData.RunningSpeedRandomDeviation);
		NewSpeed = MovementData.RunningSpedDefault + DeltaSpeed;
	}
	else
	{
		DeltaSpeed = FMath::FRandRange(-MovementData.WalkingSpeedRandomDeviation,
		                               MovementData.WalkingSpeedDefault);
		NewSpeed = MovementData.WalkingSpeedDefault + DeltaSpeed;
	}

	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}
