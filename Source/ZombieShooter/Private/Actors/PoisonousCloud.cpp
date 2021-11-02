// Created by Artyom Volkov during the UE4 course


#include "Actors/PoisonousCloud.h"

#include "Characters/PlayerCharacter.h"
#include "Components/TriggerComponents/BaseSphereTriggerComponent.h"
#include "Kismet/GameplayStatics.h"

APoisonousCloud::APoisonousCloud()
{
	PrimaryActorTick.bCanEverTick = true;

	DamageTrigger = CreateDefaultSubobject<UBaseSphereTriggerComponent>("DamageTrigger");
}

void APoisonousCloud::BeginPlay()
{
	Super::BeginPlay();

	DamageTrigger->OnComponentBeginOverlap.AddDynamic(this, &APoisonousCloud::OnDamageTriggerBeginOverlap);
	DamageTrigger->OnComponentEndOverlap.AddDynamic(this, &APoisonousCloud::OnDamageTriggerEndOverlap);

	TimeBetweenTicks = DamageRate <= 0.f ? 1.f : 1.f / DamageRate;
}

void APoisonousCloud::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APoisonousCloud::OnDamageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                                  AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp,
                                                  int32 OtherBodyIndex,
                                                  bool bFromSweep,
                                                  const FHitResult& SweepResult)
{
	if (!OtherActor->IsA(APlayerCharacter::StaticClass())) return;

	TargetActor = OtherActor;

	if (!TargetActor) return;

	StartDamageTimer();
}

void APoisonousCloud::OnDamageTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                                AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp,
                                                int32 OtherBodyIndex)
{
	if (!TargetActor || !TargetActor->IsA(APlayerCharacter::StaticClass())) return;

	StopDamageTimer();

	TargetActor = nullptr;
}

void APoisonousCloud::StartDamageTimer()
{
	if (!GetWorld()) return;

	GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &APoisonousCloud::DealDamage, TimeBetweenTicks, true);
}

void APoisonousCloud::StopDamageTimer()
{
	if (!GetWorld()) return;

	GetWorldTimerManager().ClearTimer(DamageTimerHandle);
}

void APoisonousCloud::DealDamage()
{
	if (!IsValid(TargetActor)) return;

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult,
	                                     GetActorLocation(),
	                                     TargetActor->GetActorLocation(),
	                                     ECC_GameTraceChannel1);

	if (HitResult.bBlockingHit) return;
	
	FCollisionObjectQueryParams CollisionObjectQueryParams = FCollisionObjectQueryParams(ECC_GameTraceChannel2);
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByObjectType(HitResult,
	                                        GetActorLocation(),
	                                        TargetActor->GetActorLocation(),
	                                        CollisionObjectQueryParams,
	                                        CollisionQueryParams);

	if (!HitResult.bBlockingHit) return;
	
	UGameplayStatics::ApplyPointDamage(TargetActor,
	                                   Damage,
	                                   (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal(),
	                                   HitResult,
	                                   nullptr,
	                                   this,
	                                   DamageType);
}
