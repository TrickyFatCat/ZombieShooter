// Created by Artyom Volkov during the UE4 course


#include "Components/FlashlightComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

namespace Flashlight
{
	constexpr float TickDuration = 0.15f;
	constexpr float TraceDistanceMin = 25.f;
	constexpr float TraceDistanceMax = 150.f;
	constexpr float IntensityInterpSpeed = 650.f;
}

UFlashlightComponent::UFlashlightComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFlashlightComponent::TickComponent(float DeltaTime,
                                         ELevelTick Tick,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, Tick, ThisTickFunction);

	float Alpha = 0.f;

	if (bIsEnabled)
	{
		// Calculate trace points.
		const FVector TraceStart = GetComponentLocation() + GetForwardVector() * Flashlight::TraceDistanceMin;
		const FVector TraceEnd = TraceStart + GetForwardVector() * Flashlight::TraceDistanceMax;

		// Make trace.
		GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, CollisionQueryParams);

		// Calculate intensity alpha.
		const float DistanceToImpact = FVector::DistSquared(TraceStart, TraceHit.ImpactPoint);
		const float MaxDistance = FVector::DistSquared(TraceStart, TraceEnd);
		Alpha = FMath::Clamp(DistanceToImpact / MaxDistance, 0.f, 1.f);
	}

	// Calculate intensity.
	const float TargetIntensity = FMath::Lerp(bIsEnabled ? MinIntensity : 0.f, MaxIntensity, Alpha);
	const float NewIntensity = FMath::FInterpConstantTo(Intensity,
	                                                    TargetIntensity,
	                                                    Tick,
	                                                    Flashlight::IntensityInterpSpeed);
	SetIntensity(NewIntensity);
}

void UFlashlightComponent::BeginPlay()
{
	Super::BeginPlay();

	CollisionQueryParams.AddIgnoredActor(GetOwner());
	MaxIntensity = Intensity;
	SetIntensity(0.f);
	bIsEnabled = false;
}

void UFlashlightComponent::Switch()
{
	bIsEnabled ? Disable() : Enable();
}

void UFlashlightComponent::Enable()
{
	if (bIsEnabled) return;

	bIsEnabled = true;
	UGameplayStatics::PlaySound2D(GetWorld(), SwitchOnSound);
}

void UFlashlightComponent::Disable()
{
	if (!bIsEnabled) return;

	bIsEnabled = false;
	UGameplayStatics::PlaySound2D(GetWorld(), SwitchOffSound);
}
