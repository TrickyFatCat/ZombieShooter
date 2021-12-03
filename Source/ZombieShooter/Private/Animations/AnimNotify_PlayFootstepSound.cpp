// Created by Artyom Volkov


#include "Animations/AnimNotify_PlayFootstepSound.h"
#include "Sound/SoundCue.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

namespace
{
	constexpr int32 TraceDistance = 25.0f;
}

UAnimNotify_PlayFootstepSound::UAnimNotify_PlayFootstepSound()
{
}

void UAnimNotify_PlayFootstepSound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UpdateCurrentSurface(MeshComp->GetOwner()->GetWorld(), MeshComp->GetBoneLocation(TargetBoneName));

	Sound = FootstepSounds.Contains(CurrentSurface) ? FootstepSounds[CurrentSurface] : nullptr;

	Super::Notify(MeshComp, Animation);
}

void UAnimNotify_PlayFootstepSound::UpdateCurrentSurface(UWorld* World, const FVector& TraceStart)
{
	FCollisionObjectQueryParams CollisionObjectQueryParams;
	CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.bReturnPhysicalMaterial = true;

	const FVector TraceEnd = TraceStart - FVector::UpVector * TraceDistance;

	World->LineTraceSingleByObjectType(TraceHit,
	                                   TraceStart,
	                                   TraceEnd,
	                                   CollisionObjectQueryParams,
	                                   CollisionQueryParams);

	if (!TraceHit.bBlockingHit || !TraceHit.PhysMaterial.IsValid()) return;

	CurrentSurface = TraceHit.PhysMaterial->SurfaceType;
}
