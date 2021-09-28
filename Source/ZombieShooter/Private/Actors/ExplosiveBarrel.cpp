// Created by Artyom Volkov during the UE4 course


#include "Actors/ExplosiveBarrel.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ZombieShooter/ZombieShooter.h"
#include "Components/StaticMeshComponent.h"
#include "Weapons/Components/WeaponFXComponent.h"


AExplosiveBarrel::AExplosiveBarrel()
{
	PrimaryActorTick.bCanEverTick = true;

	BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>("BarrelMesh");
	SetRootComponent(BarrelMesh);

	ExplosionFXComponent = CreateDefaultSubobject<UWeaponFXComponent>("ExplosionFXComponent");
}

void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &AExplosiveBarrel::OnTakeDamage);
}

void AExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AExplosiveBarrel::OnTakeDamage(AActor* DamagedActor,
                                    float Damage,
                                    const UDamageType* DamageType,
                                    AController* InstigatedBy,
                                    AActor* DamageCauser)
{
	if (bIsExploded) return;

	ExplosionDelay = ExplosionDelay <= 0.f ? GetWorld()->GetTimeSeconds() : ExplosionDelay;
	GetWorldTimerManager().SetTimer(ExplosionDelayHandle,
	                                this,
	                                &AExplosiveBarrel::ProcessExplosion,
	                                ExplosionDelay,
	                                false);
	
	bIsExploded = true;
}

void AExplosiveBarrel::ProcessExplosion()
{
	UGameplayStatics::ApplyRadialDamage(GetWorld(),
	                                    ExplosionDamage,
	                                    GetActorLocation(),
	                                    ExplosionRadius,
	                                    DamageTypeClass,
	                                    {},
	                                    this,
	                                    nullptr,
	                                    bDoFullDamage,
	                                    COLLISION_EXPLOSION);

	FHitResult HitResult;
	HitResult.ImpactPoint = GetActorLocation();
	HitResult.ImpactNormal = GetActorUpVector();
	ExplosionFXComponent->PlayImpactFX(HitResult);
	Destroy();
}
