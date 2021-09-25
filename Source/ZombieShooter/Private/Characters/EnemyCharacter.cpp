// Created by Artyom Volkov during the UE4 course


#include "Characters/EnemyCharacter.h"

#include "BrainComponent.h"
#include "Characters/Controllers/ZombieAIController.h"
#include "Components/ShooterDamageControllerComponent.h"
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

	OnTakeAnyDamage.AddDynamic(this, &AEnemyCharacter::OnTakeDamage);
}

void AEnemyCharacter::OnDeath(AController* DeathInstigator, AActor* DeathCauser, const UDamageType* DamageType)
{
	Super::OnDeath(DeathInstigator, DeathCauser, DamageType);

	SetLifeSpan(DefaultLifeSpan);
	AAIController* AIController = Cast<AAIController>(Controller);

	if (AIController && AIController->BrainComponent)
	{
		AIController->BrainComponent->Cleanup();
		AIController->StopMovement();
	}
	
	GetMesh()->SetSimulatePhysics(true);
}

bool AEnemyCharacter::IsRunning() const
{
	return GetCharacterMovement()->MaxWalkSpeed > MovementData.WalkingSpeedDefault + MovementData.
		WalkingSpeedRandomDeviation;
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

void AEnemyCharacter::SetIsStunned(const bool Value)
{
	if (bIsStunned == Value) return;

	AAIController* AIController = Cast<AAIController>(GetController());

	if (!AIController) return;
	
	bIsStunned = Value;

	if (bIsStunned)
	{
		PlayAnimMontage(HitReactionMontage);
		AIController->StopMovement();
	}
}

void AEnemyCharacter::OnTakeDamage(AActor* DamageActor,
                                   float Damage,
                                   const UDamageType* DamageType,
                                   AController* InstigatedBy,
                                   AActor* DamageCauser)
{
	if (DamageControllerComponent->GetIsDead() || bIsStunned || !StunChancesMap.Contains(DamageType->GetClass()))
		return;

	if (!HitReactionMontage) return;

	const float DiceRoll = FMath::FRand();

	if (DiceRoll <= StunChancesMap[DamageType->GetClass()])
	{
		SetIsStunned(true);
	}
}
