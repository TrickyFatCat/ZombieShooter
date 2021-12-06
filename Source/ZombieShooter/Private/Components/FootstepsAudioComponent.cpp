// Created by Artyom Volkov


#include "Components/FootstepsAudioComponent.h"
#include "GameFramework/Character.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

namespace Footsteps
{
	constexpr float TickDuration = 0.15f;
	constexpr float TraceDistance = 35.f;
}

UFootstepsAudioComponent::UFootstepsAudioComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetComponentTickInterval(Footsteps::TickDuration);
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
	if (!FootstepsSounds.Contains(CurrentSurface) || !GetWorld()) return;

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (TimerManager.IsTimerActive(SoundPauseHandle)) return;

	TimerManager.SetTimer(SoundPauseHandle,
	                      this,
	                      &UFootstepsAudioComponent::PlaySound,
	                      TimeBetweenFootsteps,
	                      true);
}

void UFootstepsAudioComponent::StopPlayingSound()
{
	if (!GetWorld() || !GetWorld()->GetTimerManager().IsTimerActive(SoundPauseHandle)) return;

	GetWorld()->GetTimerManager().ClearTimer(SoundPauseHandle);
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
	const FVector EndPoint = StartPoint - FVector::UpVector * Footsteps::TraceDistance;

	GetWorld()->LineTraceSingleByObjectType(TraceHit,
	                                        StartPoint,
	                                        EndPoint,
	                                        CollisionObjectQueryParams,
	                                        CollisionQueryParams);

	if (!TraceHit.bBlockingHit || !TraceHit.PhysMaterial.IsValid()) return;

	CurrentSurface = TraceHit.PhysMaterial->SurfaceType;
}

void UFootstepsAudioComponent::PlaySound()
{
	if (IsPlaying()) return;

	Sound = FootstepsSounds[CurrentSurface];
	Play();
}
