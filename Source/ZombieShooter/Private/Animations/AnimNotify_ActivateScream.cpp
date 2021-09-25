// Created by Artyom Volkov during the UE4 course


#include "Animations/AnimNotify_ActivateScream.h"

#include "Characters/EnemyCharacter.h"

UAnimNotify_ActivateScream::UAnimNotify_ActivateScream()
{
}

void UAnimNotify_ActivateScream::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (!MeshComp) return;

	AEnemyCharacter* Character = Cast<AEnemyCharacter>(MeshComp->GetOwner());

	if (!Character) return;

	Character->AggroNeighbours();
}
