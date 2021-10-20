// Created by Artyom Volkov during the UE4 course


#include "Characters/PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/ShooterDamageControllerComponent.h"
#include "Core/Session/SessionGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TrickyPrototyping/Public/Components/InteractionQueueComponent.h"
#include "Components/PlayerArmsComponent.h"
#include "Components/KeyRingComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>("PlayerCamera");
	PlayerCamera->SetupAttachment(GetMesh());

	PlayerArms = CreateDefaultSubobject<UPlayerArmsComponent>("PlayerArms");
	PlayerArms->SetupAttachment(PlayerCamera);

	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("WeaponComponent");

	InteractionQueue = CreateDefaultSubobject<UInteractionQueueComponent>("InteractionQueue");

	KeyRing = CreateDefaultSubobject<UKeyRingComponent>("KeyRing");
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultMaxSpeed = GetCharacterMovement()->MaxWalkSpeed;

	DefaultInputYawScale = GetController<APlayerController>()->InputYawScale;
	DefaultInputPitchScale = GetController<APlayerController>()->InputPitchScale;

	WeaponComponent->OnEnterAds.AddDynamic(this, &APlayerCharacter::OnEnterAds);
	WeaponComponent->OnExitAds.AddDynamic(this, &APlayerCharacter::OnExitAds);

	PlayerCamera->SetRelativeScale3D(FVector(0.2, 0.2, 0.2));
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

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
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::EnterCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APlayerCharacter::ExitCrouch);

	// Aiming
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookRight", this, &APlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", PlayerArms, &UPlayerArmsComponent::SetVerticalSway);
	PlayerInputComponent->BindAxis("LookRight", PlayerArms, &UPlayerArmsComponent::SetHorizontalSway);
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
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, PlayerArms, &UPlayerArmsComponent::StartShooting);
	PlayerInputComponent->BindAction("Shoot", IE_Released, PlayerArms, &UPlayerArmsComponent::StopShooting);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &UWeaponComponent::Reload);

	DECLARE_DELEGATE_OneParam(FChooseWeaponDelegate, const int32);
	PlayerInputComponent->BindAction<FChooseWeaponDelegate>("ChooseRevolver", IE_Pressed, WeaponComponent, &UWeaponComponent::ChooseWeapon, 0);
	PlayerInputComponent->BindAction<FChooseWeaponDelegate>("ChooseSMG", IE_Pressed, WeaponComponent, &UWeaponComponent::ChooseWeapon, 1);
	PlayerInputComponent->BindAction<FChooseWeaponDelegate>("ChooseShotgun", IE_Pressed, WeaponComponent, &UWeaponComponent::ChooseWeapon, 2);
	PlayerInputComponent->BindAction<FChooseWeaponDelegate>("ChooseAssaultRifle", IE_Pressed, WeaponComponent, &UWeaponComponent::ChooseWeapon, 3);
	PlayerInputComponent->BindAction<FChooseWeaponDelegate>("ChooseSniperRifle", IE_Pressed, WeaponComponent, &UWeaponComponent::ChooseWeapon, 4);
	PlayerInputComponent->BindAction<FChooseWeaponDelegate>("ChooseLauncher", IE_Pressed, WeaponComponent, &UWeaponComponent::ChooseWeapon, 5);

	// Other
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::StartInteraction);
}

void APlayerCharacter::OnDeath(AController* DeathInstigator, AActor* DeathCauser, const UDamageType* Damage)
{
	Super::OnDeath(DeathInstigator, DeathCauser, Damage);

	if (GetWorld())
	{
		ASessionGameMode* SessionGameMode = Cast<ASessionGameMode>(GetWorld()->GetAuthGameMode());
		
		if (SessionGameMode)
		{
			SessionGameMode->FinishSession();
		}
	}
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

void APlayerCharacter::EnterCrouch()
{
	Crouch();
}

void APlayerCharacter::ExitCrouch()
{
	UnCrouch();
}

void APlayerCharacter::AddCameraRecoil(const float RecoilPitch, const float RecoilYaw)
{
	AddControllerPitchInput(-RecoilPitch);
	AddControllerYawInput(FMath::FRandRange(-RecoilYaw, RecoilYaw));
}

void APlayerCharacter::StartInteraction()
{
	InteractionQueue->Interact();
}
