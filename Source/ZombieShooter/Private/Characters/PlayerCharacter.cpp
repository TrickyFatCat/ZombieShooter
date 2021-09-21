// Created by Artyom Volkov during the UE4 course


#include "Characters/PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/ShooterDamageControllerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>("PlayerCamera");
	PlayerCamera->SetupAttachment(GetRootComponent());

	PlayerArms = CreateDefaultSubobject<USkeletalMeshComponent>("PlayerArms");
	PlayerArms->SetupAttachment(PlayerCamera);
	PlayerArms->CastShadow = false;
	
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("WeaponComponent");
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitialRotation = PlayerArms->GetRelativeRotation();
	DefaultMaxSpeed = GetCharacterMovement()->MaxWalkSpeed;

	DefaultInputYawScale = GetController<APlayerController>()->InputYawScale;
	DefaultInputPitchScale = GetController<APlayerController>()->InputPitchScale;

	WeaponComponent->OnEnterAds.AddDynamic(this, &APlayerCharacter::OnEnterAds);
	WeaponComponent->OnExitAds.AddDynamic(this, &APlayerCharacter::OnExitAds);
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	ProcessSwayRotation(DeltaSeconds);

	// const FString Separator = TEXT("\n=====================\n");
	const FString PlayerDataHeader = TEXT("==== PLAYER DATA ====");
	const FString WeaponDataHeader = TEXT("==== WEAPON DATA ====");
	FWeaponAmmoData AmmoData;
	WeaponComponent->GetCurrentWeaponAmmo(AmmoData);

	const FString StorageAmmoText = FString::Printf(
		TEXT("Clip Ammo: %04d/%04d"),
		AmmoData.StorageAmmoCurrent,
		AmmoData.StorageAmmoMax);
	GEngine->AddOnScreenDebugMessage(6, 0.f, FColor::Orange, StorageAmmoText, true, FVector2D(1, 1));

	const FString ClipAmmoText = FString::Printf(
		TEXT("Clip Ammo: %04d/%04d"),
		AmmoData.ClipAmmoCurrent,
		AmmoData.ClipAmmoMax);
	GEngine->AddOnScreenDebugMessage(5, 0.f, FColor::Yellow, ClipAmmoText, true, FVector2D(1, 1));

	const FString WeaponText = FString::Printf(TEXT("Current weapon %s"),
	                                           *WeaponComponent->GetCurrentWeapon()->GetHumanReadableName());
	GEngine->AddOnScreenDebugMessage(4, 0.f, FColor::Red, WeaponText, true, FVector2D(1, 1));

	GEngine->AddOnScreenDebugMessage(3, 0.f, FColor::Silver, WeaponDataHeader, true, FVector2D(1, 1));

	// GEngine->AddOnScreenDebugMessage(3, 0.f, FColor::White, Separator, true, FVector2D(1, 1));

	const FString HealthText = FString::Printf(
		TEXT("Health: %4.2f/%4.2f"),
		DamageControllerComponent->GetHealth(),
		DamageControllerComponent->GetMaxHealth());
	GEngine->AddOnScreenDebugMessage(2, 0.f, FColor::Emerald, HealthText, true, FVector2D(1, 1));

	const FString ArmorText = FString::Printf(TEXT("Armor: %4.2f/%4.2f"),
	                                          DamageControllerComponent->GetArmor(),
	                                          DamageControllerComponent->GetMaxArmor());
	GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Cyan, ArmorText, true, FVector2D(1, 1));

	GEngine->AddOnScreenDebugMessage(0, 0.f, FColor::Silver, PlayerDataHeader, true, FVector2D(1, 1));
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::Jump);

	// Aiming
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookRight", this, &APlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::SetVerticalSway);
	PlayerInputComponent->BindAxis("LookRight", this, &APlayerCharacter::SetHorizontalSway);
	PlayerInputComponent->BindAction("ADS", IE_Pressed, WeaponComponent, &UWeaponComponent::EnterAds);
	PlayerInputComponent->BindAction("ADS", IE_Released, WeaponComponent, &UWeaponComponent::ExitAds);

	// Weapon
	PlayerInputComponent->BindAction("EquipNextWeapon",
	                                 IE_Pressed,
	                                 WeaponComponent,
	                                 &UWeaponComponent::EquipNextWeapon);
	PlayerInputComponent->BindAction("EquipPreviousWeapon",
	                                 IE_Pressed,
	                                 WeaponComponent,
	                                 &UWeaponComponent::EquipPreviousWeapon);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, WeaponComponent, &UWeaponComponent::StartShooting);
	PlayerInputComponent->BindAction("Shoot", IE_Released, WeaponComponent, &UWeaponComponent::StopShooting);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &UWeaponComponent::Reload);
}

void APlayerCharacter::MoveForward(const float AxisValue)
{
	AddMovementInput(GetActorForwardVector(), AxisValue);
}

void APlayerCharacter::MoveRight(const float AxisValue)
{
	AddMovementInput(GetActorRightVector(), AxisValue);
}

void APlayerCharacter::OnEnterAds()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxSpeed * WeaponComponent->GetMovementFactor();
	GetController<APlayerController>()->InputYawScale = DefaultInputYawScale * WeaponComponent->GetAimFactor();
	GetController<APlayerController>()->InputPitchScale = DefaultInputPitchScale * WeaponComponent->GetAimFactor();
}

void APlayerCharacter::OnExitAds()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxSpeed;
	GetController<APlayerController>()->InputYawScale = DefaultInputYawScale;
	GetController<APlayerController>()->InputPitchScale = DefaultInputPitchScale;
}

void APlayerCharacter::SetHorizontalSway(const float AxisValue)
{
	FRotator FinalRotation = PlayerArms->GetRelativeRotation();
	FinalRotation.Yaw = FinalRotation.Yaw - AxisValue * SwayPower;
	PlayerArms->SetRelativeRotation(FinalRotation);
}

void APlayerCharacter::SetVerticalSway(const float AxisValue)
{
	FRotator FinalRotation = PlayerArms->GetRelativeRotation();
	FinalRotation.Roll = FinalRotation.Roll - AxisValue * SwayPower;
	PlayerArms->SetRelativeRotation(FinalRotation);
}

void APlayerCharacter::ProcessSwayRotation(const float DeltaTime) const
{
	const FRotator TargetRotation = FRotator(PlayerArms->GetRelativeRotation().Pitch,
	                                         InitialRotation.Yaw,
	                                         InitialRotation.Roll);
	const FRotator FinalRotation = FMath::RInterpTo(PlayerArms->GetRelativeRotation(),
	                                                TargetRotation,
	                                                DeltaTime,
	                                                SwaySpeed);
	PlayerArms->SetRelativeRotation(FinalRotation);
}

void APlayerCharacter::AddCameraRecoil(const float RecoilPitch, const float RecoilYaw)
{
	AddControllerPitchInput(-RecoilPitch);
	AddControllerYawInput(FMath::FRandRange(-RecoilYaw, RecoilYaw));
}
