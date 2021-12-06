// Created by Artyom Volkov during the UE4 course


#include "Components/FlashlightComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

#include "DrawDebugHelpers.h"

namespace Flashlight
{
	constexpr float TickDuration = 0.15f;
	constexpr float TraceDistance = 150.f;
	constexpr float TraceRadius = 25.f;
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

	if (bHiddenInGame) return;
	const FVector TraceStart = GetComponentLocation() + GetForwardVector() * 25.f;
	const FVector TraceEnd = TraceStart + GetForwardVector() * Flashlight::TraceDistance;

	UKismetSystemLibrary::SphereTraceSingle(GetWorld(),
	                                        TraceStart,
	                                        TraceEnd,
	                                        Flashlight::TraceRadius,
	                                        UEngineTypes::ConvertToTraceType(ECC_Visibility),
	                                        false,
	                                        { GetOwner() },
	                                        EDrawDebugTrace::None,
	                                        TraceHit,
	                                        true);

	const float DistanceToImpact = FVector::DistSquared(TraceStart, TraceHit.ImpactPoint);
	const float MaxDistance = FVector::DistSquared(TraceStart, TraceEnd);
	const float Alpha = FMath::Clamp(DistanceToImpact / MaxDistance, 0.f, 1.f);
	const float TargetIntensity = FMath::Lerp(MinIntensity, MaxIntensity, Alpha);
	const float NewIntensity = FMath::FInterpConstantTo(Intensity, TargetIntensity, Tick, 750);
	SetIntensity(NewIntensity);
}

void UFlashlightComponent::Switch()
{
	bHiddenInGame ? Enable() : Disable();
}

void UFlashlightComponent::BeginPlay()
{
	Super::BeginPlay();

	MaxIntensity = Intensity;
	Disable();
}

void UFlashlightComponent::Enable()
{
	if (!bHiddenInGame) return;

	SetHiddenInGame(false);
	UGameplayStatics::PlaySound2D(GetWorld(), SwitchOnSound);
}

void UFlashlightComponent::Disable()
{
	if (bHiddenInGame) return;

	SetHiddenInGame(true);
	SetIntensity(0.f);
	UGameplayStatics::PlaySound2D(GetWorld(), SwitchOffSound);
}
