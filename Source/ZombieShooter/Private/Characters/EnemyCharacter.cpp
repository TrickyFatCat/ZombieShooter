// Created by Artyom Volkov during the UE4 course


#include "Characters/EnemyCharacter.h"

#include "BrainComponent.h"
#include "Animation/AnimInstanceProxy.h"
#include "AI/AIControllerBase.h"
#include "Components/ShooterDamageControllerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/TriggerComponents/DamageSphereComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = AAIControllerBase::StaticClass();
	bUseControllerRotationYaw = false;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 200.f, 0.f);
	}

	DamageTrigger = CreateDefaultSubobject<UDamageSphereComponent>("DamageTrigger");
	DamageTrigger->SetupAttachment(GetMesh(), AttackSocketName);
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &AEnemyCharacter::OnTakeDamage);
	DamageTrigger->SetIsEnabled(true);
	DamageTrigger->SetDamage(AttackDamage);
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

void AEnemyCharacter::AttackPlayer() const
{
	AAIControllerBase* AIController = Cast<AAIControllerBase>(GetController());

	if (!AIController) return;

	AIController->StartAttacking(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	SetIsRunning(true);
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

void AEnemyCharacter::StartAttack()
{
	DamageTrigger->SetIsEnabled(true);
}

void AEnemyCharacter::FinishAttack()
{
	DamageTrigger->SetIsEnabled(false);
}

AActor* AEnemyCharacter::GetTargetActor() const
{
	AAIControllerBase* AIControllerBase = Cast<AAIControllerBase>(GetController());

	if (!AIControllerBase) return nullptr;

	return AIControllerBase->GetTargetActor();
}
