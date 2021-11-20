// Created by Artyom Volkov during the UE4 course


#include "Actors/EnemySpawns/EnemySpawnArea.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

AEnemySpawnArea::AEnemySpawnArea()
{
	SpawnArea = CreateDefaultSubobject<UBoxComponent>("SpawnArea");
	SetRootComponent(SpawnArea);
	SpawnArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SpawnArea->SetGenerateOverlapEvents(false);
}

FTransform AEnemySpawnArea::CalculateSpawnTransform()
{
	const FVector VolumeExtent = SpawnArea->GetScaledBoxExtent();
	const FVector VolumeOrigin = SpawnArea->GetComponentLocation();
	const FVector Location = UKismetMathLibrary::RandomPointInBoundingBox(VolumeOrigin, VolumeExtent);
	const FRotator Rotation = FRotator(0.f, FMath::FRandRange(0.f, 360.f), 0.f);
	return FTransform(Rotation, FVector(Location.X, Location.Y, GetActorLocation().Z), FVector::OneVector);
}
