// Created by Artyom Volkov during the UE4 course


#include "Weapons/ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/Components/WeaponFXComponent.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileCollision = CreateDefaultSubobject<USphereComponent>("ProjectileCollision");
	SetRootComponent(ProjectileCollision);
	ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	ProjectileCollision->bReturnMaterialOnMove = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->bRotationFollowsVelocity = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileMesh");
	ProjectileMesh->SetupAttachment(GetRootComponent());

	ProjectileFX = CreateDefaultSubobject<UWeaponFXComponent>("ProjectileFX");
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMovement->Velocity = ShotDirection * ProjectileMovement->InitialSpeed;
	ProjectileCollision->IgnoreActorWhenMoving(GetOwner(), true);
	ProjectileCollision->OnComponentHit.AddDynamic(this, &AProjectileBase::OnProjectileHit);
	SetInstigator(Cast<APawn>(GetOwner()));
	SetLifeSpan(DefaultLifeSpan);

	if (ProjectileData.bIsExplosive && !ProjectileData.bDamageOwner)
	{
		IgnoredActors.Add(GetOwner());
	}
}

void AProjectileBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (EndPlayReason != EEndPlayReason::Destroyed) return;
	
	if (GetLifeSpan() <= 0.f && ProjectileData.bIsExplosive)
	{
		DealRadialDamage();
		FHitResult HitResult;
		HitResult.ImpactPoint = GetActorLocation();
		ProjectileFX->PlayImpactFX(HitResult);
	}
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileBase::GetProjectileData(FProjectileData& Data) const
{
	Data = ProjectileData;
}

void AProjectileBase::SetDirectionAndDamage(const FVector& Direction, const int32 Damage)
{
	ShotDirection = Direction;

	if (Damage <= 0) return;

	ProjectileData.Damage = Damage;
}

void AProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComponent,
                                      AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp,
                                      FVector NormalImpulse,
                                      const FHitResult& Hit)
{
	UWorld* World = GetWorld();

	if (!World) return;

	ProjectileMovement->StopMovementImmediately();

	if (ProjectileData.bIsExplosive)
	{
		DealRadialDamage();
	}
	else
	{
		UGameplayStatics::ApplyPointDamage(OtherActor,
		                                   ProjectileData.Damage,
		                                   Hit.Location,
		                                   Hit,
		                                   GetInstigatorController(),
		                                   this,
		                                   UDamageType::StaticClass());
	}

	ProjectileFX->PlayImpactFX(Hit);
	Destroy();
}

void AProjectileBase::DealRadialDamage()
{
	if (!GetWorld()) return;

	UE_LOG(LogTemp, Warning, TEXT("KABOOM"));

	UGameplayStatics::ApplyRadialDamage(GetWorld(),
	                                    ProjectileData.Damage,
	                                    GetActorLocation(),
	                                    ProjectileData.ExplosionRadius,
	                                    UDamageType::StaticClass(),
	                                    IgnoredActors,
	                                    this,
	                                    GetInstigatorController(),
	                                    ProjectileData.bDealFullDamage);
}
