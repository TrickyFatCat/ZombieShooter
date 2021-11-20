// Created by Artyom Volkov during the UE4 course


#include "Weapons/Components/WeaponFXComponent.h"

#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Sound/SoundCue.h"

UWeaponFXComponent::UWeaponFXComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UWeaponFXComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UWeaponFXComponent::TickComponent(float DeltaTime,
                                       ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UWeaponFXComponent::PlayImpactFX(const FHitResult& HitResult)
{
	UWorld* World = GetWorld();

	if (!World) return;

	FHitScanImpactData ImpactData = DefaultImpactData;

	if (HitResult.PhysMaterial.IsValid())
	{
		const EPhysicalSurface SurfaceType = HitResult.PhysMaterial->SurfaceType;

		if (ImpactDataMap.Contains(SurfaceType))
		{
			ImpactData = ImpactDataMap[SurfaceType];
		}
	}

	UGameplayStatics::SpawnEmitterAtLocation(World,
	                                         ImpactData.ImpactEffect,
	                                         HitResult.ImpactPoint,
	                                         HitResult.ImpactNormal.Rotation());

	UGameplayStatics::PlaySoundAtLocation(World, ImpactData.Sound, HitResult.ImpactPoint);

	UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(World,
	                                                                ImpactData.DecalData.Material,
	                                                                ImpactData.DecalData.Size,
	                                                                HitResult.ImpactPoint,
	                                                                HitResult.ImpactNormal.Rotation());

	if (Decal)
	{
		Decal->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeOutDuration);
	}
}
