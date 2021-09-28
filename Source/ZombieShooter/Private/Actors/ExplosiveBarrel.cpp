// Created by Artyom Volkov during the UE4 course


#include "Actors/ExplosiveBarrel.h"

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
	// OnTakeRadialDamage.AddDynamic(this, &AExplosiveBarrel::OnRadialDamage);
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

	bIsExploded = true;
	
	Destroy();
}

void AExplosiveBarrel::OnRadialDamage(AActor* DamagedActor,
                                      float Damage,
                                      const UDamageType* DamageType,
                                      FVector Origin,
                                      FHitResult HitInfo,
                                      AController* InstigatedBy,
                                      AActor* DamageCauser)
{
	UGameplayStatics::ApplyRadialDamage(GetWorld(),
	                                    ExplosionDamage,
	                                    GetActorLocation(),
	                                    ExplosionRadius,
	                                    DamageTypeClass,
	                                    {},
	                                    this,
	                                    InstigatedBy,
	                                    bDoFullDamage,
	                                    COLLISION_EXPLOSION);

	FHitResult HitResult;
	HitResult.ImpactPoint = GetActorLocation();
	HitResult.ImpactNormal = GetActorUpVector();
	ExplosionFXComponent->PlayImpactFX(HitResult);

	Destroy();
}
