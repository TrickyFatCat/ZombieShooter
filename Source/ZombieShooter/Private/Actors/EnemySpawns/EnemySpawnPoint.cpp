// Created by Artyom Volkov during the UE4 course


#include "Actors/EnemySpawns/EnemySpawnPoint.h"
#include "Components/ArrowComponent.h"

AEnemySpawnPoint::AEnemySpawnPoint()
{
	ForwardVectorPointer = CreateDefaultSubobject<UArrowComponent>("ForwardVectorPointer");
	SetRootComponent(ForwardVectorPointer);
}

FTransform AEnemySpawnPoint::CalculateSpawnTransform()
{
	return FTransform(FRotator(0.f, GetActorRotation().Yaw, 0.f), GetActorLocation(), FVector::OneVector);
}
