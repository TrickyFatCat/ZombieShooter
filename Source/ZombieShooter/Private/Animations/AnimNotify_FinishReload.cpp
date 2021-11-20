// Created by Artyom Volkov during the UE4 course


#include "Animations/AnimNotify_FinishReload.h"

#include "Weapons/WeaponBase.h"

void UAnimNotify_FinishReload::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (!MeshComp) return;

	AWeaponBase* Weapon = Cast<AWeaponBase>(MeshComp->GetOwner());

	if (!Weapon) return;

	Weapon->Reload();
}
