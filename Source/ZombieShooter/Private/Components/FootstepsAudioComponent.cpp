// Created by Artyom Volkov


#include "Components/FootstepsAudioComponent.h"
#include "GameFramework/Character.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

constexpr float TickDuration = 0.1f;
constexpr float TraceDistance = 35.f;

UFootstepsAudioComponent::UFootstepsAudioComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetComponentTickInterval(TickDuration);
}

void UFootstepsAudioComponent::TickComponent(float DeltaTime,
                                        ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateSurface();
}

void UFootstepsAudioComponent::StartPlayingSound()
{
	UpdateSurface();
	if (!FootstepsSounds.Contains(CurrentSurface) || IsPlaying()) return;

	Sound = FootstepsSounds[CurrentSurface];
	Play();
}

void UFootstepsAudioComponent::StopPlayingSound()
{
	if (!IsPlaying()) return;
	
	Stop();
}

void UFootstepsAudioComponent::UpdateSurface()
{
	FCollisionObjectQueryParams CollisionObjectQueryParams;
	CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.bReturnPhysicalMaterial = true;
	CollisionQueryParams.AddIgnoredActor(GetOwner());

	const FVector StartPoint = GetComponentLocation();
	const FVector EndPoint = StartPoint - FVector::UpVector * TraceDistance;

	GetWorld()->LineTraceSingleByObjectType(TraceHit,
	                                        StartPoint,
	                                        EndPoint,
	                                        CollisionObjectQueryParams,
	                                        CollisionQueryParams);

	if (!TraceHit.bBlockingHit || !TraceHit.PhysMaterial.IsValid()) return;

	CurrentSurface = TraceHit.PhysMaterial->SurfaceType;
}
