// Created by Artyom Volkov


#include "Components/FootstepsAudioComponent.h"
#include "GameFramework/Character.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

#include "DrawDebugHelpers.h"

UFootstepsAudioComponent::UFootstepsAudioComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFootstepsAudioComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UFootstepsAudioComponent::TickComponent(float DeltaTime,
                                        ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckSurface(TraceHit);
	SetCurrentSurface(TraceHit);
}

void UFootstepsAudioComponent::StartPlayingSound()
{
	if (!FootstepsSounds.Contains(CurrentSurface) || IsPlaying()) return;

	Sound = FootstepsSounds[CurrentSurface];
	Play();
}

void UFootstepsAudioComponent::StopPlayingSound()
{
	if (!IsPlaying()) return;
	
	Stop();
}

void UFootstepsAudioComponent::CheckSurface(FHitResult& HitResult) const
{
	FCollisionObjectQueryParams CollisionObjectQueryParams;
	CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.bReturnPhysicalMaterial = true;

	const FVector StartPoint = GetComponentLocation();
	const FVector EndPoint = StartPoint - FVector::UpVector * 25.f;

	GetWorld()->LineTraceSingleByObjectType(HitResult,
	                                        StartPoint,
	                                        EndPoint,
	                                        CollisionObjectQueryParams,
	                                        CollisionQueryParams);

	DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Cyan, false, 0.25f);
}

void UFootstepsAudioComponent::SetCurrentSurface(const FHitResult& HitResult)
{
	if (!HitResult.PhysMaterial.IsValid()) return;

	CurrentSurface = HitResult.PhysMaterial->SurfaceType;
}
