// Created by Artyom Volkov during the UE4 course


#include "Animations/AnimNotify_EnemyStartAttack.h"
#include "Characters/EnemyCharacter.h"

void UAnimNotify_EnemyStartAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	
	if (!MeshComp) return;

	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(MeshComp->GetOwner());

	if (!EnemyCharacter) return;

	EnemyCharacter->StartAttack(AttackType);
}
