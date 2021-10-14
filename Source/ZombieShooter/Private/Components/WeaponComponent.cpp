// Created by Artyom Volkov during the UE4 course


#include "Components/WeaponComponent.h"

#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "Characters/PlayerCharacter.h"
#include "Components/TimelineComponent.h"
#include "Weapons/WeaponBase.h"
#include "Core/ProjectUtils.h"
#include "Kismet/KismetMaterialLibrary.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	PullAnimationTimeline = CreateDefaultSubobject<UTimelineComponent>("PullAnimationTimeline");
	RecoilTimeline = CreateDefaultSubobject<UTimelineComponent>("RecoilAnimationTimeline");
	AdsTransitionTimeline = CreateDefaultSubobject<UTimelineComponent>("AdsTransitionTimeline");
}


void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapons();

	if (Weapons.Num() > 0)
	{
		Weapons[0].bIsAvailable = true;
		EquipWeapon(CurrentWeaponIndex);
	}

	PullAnimationTimeline->SetPlayRate(1.f / (PullDuration * 0.5f));
	AdsTransitionTimeline->SetPlayRate(1.f / (AdsTransitionDuration * 0.5f));

	if (PullAnimationCurve)
	{
		FOnTimelineFloat AnimationProgress;
		AnimationProgress.BindUFunction(this, FName("SetPullProgress"));
		PullAnimationTimeline->AddInterpFloat(PullAnimationCurve, AnimationProgress);

		FOnTimelineEvent AnimationFinished;
		AnimationFinished.BindUFunction(this, FName("OnPullFinished"));
		PullAnimationTimeline->SetTimelineFinishedFunc(AnimationFinished);
	}

	if (RecoilAnimationCurve)
	{
		FOnTimelineFloat AnimationProgress;
		AnimationProgress.BindUFunction(this, FName("SetRecoilProgress"));
		RecoilTimeline->AddInterpFloat(RecoilAnimationCurve, AnimationProgress);

		FOnTimelineEvent AnimationFinished;
		AnimationFinished.BindUFunction(this, FName("OnRecoilFinished"));
		RecoilTimeline->SetTimelineFinishedFunc(AnimationFinished);
	}

	if (AdsTransitionCurve)
	{
		FOnTimelineFloat AnimationProgress;
		AnimationProgress.BindUFunction(this, FName("SetAdsTransitionProgress"));
		AdsTransitionTimeline->AddInterpFloat(AdsTransitionCurve, AnimationProgress);

		FOnTimelineEvent AnimationFinished;
		AnimationFinished.BindUFunction(this, FName("OnAdsTransitionFinished"));
		AdsTransitionTimeline->SetTimelineFinishedFunc(AnimationFinished);
	}

	TargetCamera = GetOwner()->FindComponentByClass<UCameraComponent>();
	DefaultFOV = TargetCamera->FieldOfView;
}


void UWeaponComponent::TickComponent(float DeltaTime,
                                     ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckIsNearWall();
}

void UWeaponComponent::GetCurrentWeaponData(FWeaponData& WeaponData) const
{
	CurrentWeapon->GetWeaponData(WeaponData);
}

void UWeaponComponent::SpawnWeapons()
{
	if (!GetWorld() || !GetOwner()) return;

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());

	if (!PlayerCharacter) return;

	for (auto WeaponClass : WeaponClasses)
	{
		AWeaponBase* Weapon = GetWorld()->SpawnActorDeferred<AWeaponBase>(
			WeaponClass,
			FTransform(FRotator::ZeroRotator, FVector::ZeroVector), GetOwner());

		if (!Weapon) continue;

		Weapon->OnWeaponClipEmpty.AddUObject(this, &UWeaponComponent::OnEmptyClip);
		Weapons.Add(FWeaponInventoryData{Weapon, false});
		FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
		Weapon->AttachToComponent(PlayerCharacter->GetPlayerArms(),
		                          AttachmentTransformRules);
		Weapon->SetActorHiddenInGame(true);
		Weapon->OnMakeShot.AddUObject(this, &UWeaponComponent::OnWeaponMakeShot);
		Weapon->FinishSpawning(FTransform(FRotator::ZeroRotator, FVector::ZeroVector));
	}
}

void UWeaponComponent::EquipNextWeapon()
{
	if (bIsEquipping || bIsReloading || bIsAiming) return;

	PreviousWeaponIndex = CurrentWeaponIndex;

	do
	{
		CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	}
	while (!Weapons[CurrentWeaponIndex].bIsAvailable);

	if (PreviousWeaponIndex == CurrentWeaponIndex) return;

	CurrentWeapon->StopShooting();
	StartEquipAnimation();
}

void UWeaponComponent::EquipPreviousWeapon()
{
	if (bIsEquipping || bIsReloading || bIsAiming) return;

	PreviousWeaponIndex = CurrentWeaponIndex;

	do
	{
		CurrentWeaponIndex = CurrentWeaponIndex == 0 ? Weapons.Num() - 1 : CurrentWeaponIndex - 1;
	}
	while (!Weapons[CurrentWeaponIndex].bIsAvailable);

	if (PreviousWeaponIndex == CurrentWeaponIndex) return;

	CurrentWeapon->StopShooting();
	StartEquipAnimation();
}

void UWeaponComponent::StartShooting()
{
	if (!CurrentWeapon) return;

	if (!CanShoot()) return;

	CurrentWeapon->StartShooting();
	bIsShooting = true;
}

void UWeaponComponent::StopShooting()
{
	if (!CurrentWeapon) return;

	if (!CurrentWeapon) return;

	CurrentWeapon->StopShooting();
	bIsShooting = false;
}

void UWeaponComponent::Reload()
{
	if (!CurrentWeapon || bIsEquipping || bIsReloading || AdsTransitionTimeline->IsPlaying() || !CurrentWeapon->
		CanReload())
		return;

	if (bIsAiming)
	{
		ExitAds();
	}

	CurrentWeapon->StopShooting();
	bIsReloading = true;
	PullCommand = EWeaponPullCommand::Reload;
	FWeaponData WeaponData;
	CurrentWeapon->GetWeaponData(WeaponData);

	if (WeaponData.ReloadTime <= 0.f)
	{
		CurrentWeapon->Reload();
		bIsReloading = false;
		return;
	}

	PullAnimationTimeline->PlayFromStart();
}

bool UWeaponComponent::UnlockWeapon(TSubclassOf<AWeaponBase> WeaponClass)
{
	bool Result = false;

	for (int32 i = 0; i < Weapons.Num(); ++i)
	{
		FWeaponInventoryData& InventoryData = Weapons[i];
		if (!InventoryData.Weapon->IsA(WeaponClass)) continue;

		if (InventoryData.bIsAvailable)
		{
			Result = false;
			break;
		};

		InventoryData.bIsAvailable = true;

		if (!bIsReloading && !bIsEquipping && !RecoilTimeline->IsPlaying())
		{
			PreviousWeaponIndex = CurrentWeaponIndex;
			CurrentWeaponIndex = i;
			StartEquipAnimation();
		}

		Result = true;
		break;
	}

	return Result;
}

bool UWeaponComponent::RestoreStorageAmmo(TSubclassOf<AWeaponBase> WeaponClass, const int32 Amount)
{
	bool Result = false;

	if (Amount <= 0) return Result;

	for (int32 i = 0; i < Weapons.Num(); ++i)
	{
		const FWeaponInventoryData& InventoryData = Weapons[i];

		if (!InventoryData.Weapon->IsA(WeaponClass)) continue;

		if (InventoryData.Weapon->StorageIsFull())
		{
			Result = false;
			break;
		}

		InventoryData.Weapon->IncreaseCurrentStorageAmmo(Amount);
		Result = true;
		break;
	}

	return Result;
}

void UWeaponComponent::EquipWeapon(const int32 WeaponIndex)
{
	if (!GetOwner() || WeaponIndex >= Weapons.Num() || Weapons.Num() == 0) return;

	if (CurrentWeapon)
	{
		CurrentWeapon->StopShooting();
		CurrentWeapon->SetActorHiddenInGame(true);
	}

	CurrentWeapon = Weapons[WeaponIndex].Weapon;
	CurrentWeapon->SetActorHiddenInGame(false);
	RecoilTimeline->SetPlayRate(1.f / (FMath::Min(RecoilDuration, CurrentWeapon->GetTimeBetweenShots()) * 0.5f));

	FWeaponData WeaponData;
	CurrentWeapon->GetWeaponData(WeaponData);
	AdsData = WeaponData.AdsData;
}

void UWeaponComponent::OnEmptyClip(AWeaponBase* TargetWeapon)
{
	if (!TargetWeapon) return;

	if (CurrentWeapon == TargetWeapon && !RecoilTimeline->IsPlaying())
	{
		Reload();
	}
}

void UWeaponComponent::OnReloadFinished() const
{
	CurrentWeapon->Reload();
	PullAnimationTimeline->ReverseFromEnd();
}

void UWeaponComponent::OnWeaponMakeShot()
{
	RecoilTimeline->PlayFromStart();
	OnWeaponShot.Broadcast();
}

void UWeaponComponent::CheckIsNearWall()
{
	FVector ViewLocation = FVector::ZeroVector;
	FRotator ViewRotation = FRotator::ZeroRotator;

	if (!FProjectUtils::GetPlayerViewPoint(GetOwner(), ViewLocation, ViewRotation)) return;

	FVector TraceStart = ViewLocation;
	FVector TraceDirection = ViewRotation.Vector();
	FVector TraceEnd = TraceStart + TraceDirection * WallCheckDistance;

	if (!GetWorld()) return;

	FCollisionObjectQueryParams CollisionObjectQueryParams;
	CollisionObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());
	FHitResult HitResult;

	GetWorld()->LineTraceSingleByObjectType(HitResult,
	                                        TraceStart,
	                                        TraceEnd,
	                                        CollisionObjectQueryParams,
	                                        CollisionQueryParams);

	bIsNearWall = HitResult.bBlockingHit;

	if (bIsShooting && bIsNearWall)
	{
		StopShooting();
	}

	if (bIsAiming && bIsNearWall)
	{
		ExitAds();
	}
}

void UWeaponComponent::SetRecoilProgress(const float Value)
{
	RecoilProgress = Value;

	if (RecoilTimeline->IsReversing()) return;

	APlayerCharacter* Character = Cast<APlayerCharacter>(GetOwner());

	if (!Character) return;

	FWeaponData WeaponData;
	CurrentWeapon->GetWeaponData(WeaponData);
	Character->AddCameraRecoil(WeaponData.RecoilData.CameraRecoilPitchPower * Value * RecoilMultiplier,
	                           WeaponData.RecoilData.CameraRecoilYawPower * Value * RecoilMultiplier);
}

void UWeaponComponent::OnRecoilFinished()
{
	if (RecoilProgress <= 0.f)
	{
		if (CurrentWeapon->IsClipEmpty())
		{
			Reload();
		}

		return;
	}

	RecoilTimeline->ReverseFromEnd();
}

void UWeaponComponent::OnPullFinished()
{
	FWeaponData WeaponData;
	CurrentWeapon->GetWeaponData(WeaponData);

	switch (PullCommand)
	{
	case EWeaponPullCommand::Reload:
		if (PullProgress >= 1.f)
		{
			GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle,
			                                       this,
			                                       &UWeaponComponent::OnReloadFinished,
			                                       WeaponData.ReloadTime,
			                                       false);
			return;
		}

		bIsReloading = false;

		if (bIsShooting)
		{
			StartShooting();
		}
		break;

	case EWeaponPullCommand::Equip:
		if (PullProgress >= 1.f)
		{
			EquipWeapon(CurrentWeaponIndex);
			PullAnimationTimeline->ReverseFromEnd();
			return;
		}

		bIsEquipping = false;

		if (bIsShooting)
		{
			StartShooting();
		}
		break;
	}
}

void UWeaponComponent::StartEquipAnimation()
{
	bIsEquipping = true;
	PullCommand = EWeaponPullCommand::Equip;

	if (PullDuration <= 0.f)
	{
		EquipWeapon(CurrentWeaponIndex);
		bIsEquipping = false;
		return;
	}

	PullAnimationTimeline->PlayFromStart();
}

void UWeaponComponent::GetCurrentWeaponAmmo(FWeaponAmmoData& AmmoData) const
{
	AmmoData = CurrentWeapon->GetAmmoData();
}

void UWeaponComponent::EnterAds()
{
	if (!CurrentWeapon || !TargetCamera || bIsEquipping || bIsReloading || !AdsData.bHasAds || bIsNearWall) return;

	bIsAiming = true;

	if (AdsTransitionTimeline->IsPlaying())
	{
		AdsTransitionTimeline->Reverse();
	}
	else
	{
		AdsTransitionTimeline->PlayFromStart();
	}
}

void UWeaponComponent::ExitAds()
{
	if (!CurrentWeapon || !TargetCamera || bIsEquipping || bIsReloading || !bIsAiming || !AdsData.bHasAds ||
		bIsNearWall) return;

	CurrentWeapon->SetActorHiddenInGame(false);

	if (AdsTransitionTimeline->IsPlaying())
	{
		AdsTransitionTimeline->Reverse();
	}
	else
	{
		AdsTransitionTimeline->ReverseFromEnd();
	}
}

void UWeaponComponent::SetAdsTransitionProgress(const float Value)
{
	AdsTransitionProgress = Value;
	const float NewFOV = FMath::Lerp(DefaultFOV, AdsData.TargetFOV, Value);
	TargetCamera->FieldOfView = NewFOV;

	if (!GetWorld()) return;

	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), PaniniParameterCollection, FOVParameterName, NewFOV);
}

void UWeaponComponent::OnAdsTransitionFinished()
{
	if (AdsTransitionProgress >= 1.f)
	{
		CurrentWeapon->SetActorHiddenInGame(AdsData.bIsUsingScope);
		CurrentWeapon->SetSpreadMultiplier(AdsData.SpreadFactor);
		RecoilMultiplier = AdsData.RecoilFactor;
		OnEnterAds.Broadcast();
		return;
	}

	bIsAiming = false;
	CurrentWeapon->SetSpreadMultiplier(1.f);
	RecoilMultiplier = 1.f;
	OnExitAds.Broadcast();
}
