// Created by Artyom Volkov during the UE4 course


#include "Components/FlashlightComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

#include "DrawDebugHelpers.h"

namespace Flashlight
{
	constexpr float TickDuration = 0.15f;
	constexpr float TraceDistance = 150.f;
}

UFlashlightComponent::UFlashlightComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetComponentTickInterval(Flashlight::TickDuration);
}

void UFlashlightComponent::TickComponent(float DeltaTime,
                                         ELevelTick Tick,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, Tick, ThisTickFunction);

	const FVector TraceStart = GetComponentLocation() + GetForwardVector() * 50.f;
	const FVector TraceEnd = TraceStart + GetForwardVector() * Flashlight::TraceDistance;

	UKismetSystemLibrary::SphereTraceSingle(GetWorld(),
	                                        TraceStart,
	                                        TraceEnd,
	                                        45.f,
	                                        UEngineTypes::ConvertToTraceType(ECC_Visibility),
	                                        false,
	                                        { GetOwner() },
	                                        EDrawDebugTrace::ForOneFrame,
	                                        TraceHit,
	                                        true);

	DrawDebugLine(GetWorld(),
	              TraceStart,
	              TraceEnd,
	              FColor::Red,
	              false,
	              3.f);

	const float DistanceToImpact = FVector::DistSquared(TraceStart, TraceHit.ImpactPoint);
	const float MaxDistance = FVector::DistSquared(TraceStart, TraceEnd);
	const float TargetAlpha = FMath::Clamp(DistanceToImpact / MaxDistance, 0.f, 1.f);
	float Alpha = 1.f;
	Alpha = FMath::Lerp(Alpha, TargetAlpha, 0.8f);
	// Alpha = FMath::FInterpConstantTo(Alpha, TargetAlpha, Tick, 0.5f);
	// SetIntensity(FMath::InterpEaseInOut(MinIntensity, DefaultIntensity, Alpha, 0.1));
	SetIntensity(FMath::Lerp(MinIntensity, DefaultIntensity, Alpha));
	DrawDebugPoint(GetWorld(), TraceHit.ImpactPoint, 5, FColor::Green, false, 3);

	 UE_LOG(LogTemp, Error, TEXT("Flashlight target alpha: %f | Flashlight alpha: %f | Flashlight Intensity: %f"), TargetAlpha, Alpha, Intensity);
}

void UFlashlightComponent::Switch()
{
	bHiddenInGame ? Enable() : Disable();
}

void UFlashlightComponent::BeginPlay()
{
	Super::BeginPlay();

	DefaultIntensity = Intensity;
	CollisionQueryParams.AddIgnoredActor(GetOwner());
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
	UGameplayStatics::PlaySound2D(GetWorld(), SwitchOffSound);
}
