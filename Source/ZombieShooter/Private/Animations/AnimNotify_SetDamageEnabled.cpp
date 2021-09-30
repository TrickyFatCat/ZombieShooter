// Created by Artyom Volkov during the UE4 course


#include "Animations/AnimNotify_SetDamageEnabled.h"
#include "Characters/EnemyCharacter.h"

void UAnimNotify_SetDamageEnabled::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	
	if (!MeshComp) return;

	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(MeshComp->GetOwner());

	if (!EnemyCharacter) return;

	bIsEnabled ? EnemyCharacter->StartAttack() : EnemyCharacter->FinishAttack();
}
