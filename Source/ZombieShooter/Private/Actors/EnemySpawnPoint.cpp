// Created by Artyom Volkov during the UE4 course


#include "Actors/EnemySpawnPoint.h"
#include "Components/ArrowComponent.h"

AEnemySpawnPoint::AEnemySpawnPoint()
{
	ForwardVectorPointer = CreateDefaultSubobject<UArrowComponent>("ForwardVectorPointer");
	SetRootComponent(ForwardVectorPointer);
}

FTransform AEnemySpawnPoint::CalculateSpawnTransform()
{
	return GetActorTransform();
}