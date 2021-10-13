// Created by Artyom Volkov during the UE4 course


#include "Animations/AnimNotify_MakeNoise.h"

#include "Characters/BaseCharacter.h"
#include "GameFramework/Pawn.h"

UAnimNotify_MakeNoise::UAnimNotify_MakeNoise()
{
	
}

void UAnimNotify_MakeNoise::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (!MeshComp) return;

	APawn* Pawn = Cast<APawn>(MeshComp->GetOwner());

	if (!Pawn) return;

	Pawn->MakeNoise(Loudness, Pawn, Pawn->GetActorLocation(), MaxRange, Tag);
}
