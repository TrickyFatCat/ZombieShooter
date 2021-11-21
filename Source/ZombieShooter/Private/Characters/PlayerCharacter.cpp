// Created by Artyom Volkov during the UE4 course


#include "Characters/PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/FlashlightComponent.h"
#include "Components/ShooterDamageControllerComponent.h"
#include "Core/Session/SessionGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TrickyPrototyping/Public/Components/InteractionQueueComponent.h"
#include "Components/PlayerArmsComponent.h"
#include "Components/KeyRingComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>("PlayerCamera");
	PlayerCamera->SetupAttachment(GetRootComponent());

	PlayerArms = CreateDefaultSubobject<UPlayerArmsComponent>("PlayerArms");
	PlayerArms->SetupAttachment(PlayerCamera);

	Flashlight = CreateDefaultSubobject<UFlashlightComponent>("Flashlight");
	Flashlight->SetupAttachment(PlayerCamera);

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
	CameraInitialLocation = PlayerCamera->GetRelativeLocation();
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


	if (!GetCharacterMovement()->IsFalling())
	{
		float NewZ = 0.f;
		
		if (GetCharacterMovement()->IsCrouching())
		{
			NewZ = FMath::Lerp(PlayerCamera->GetRelativeLocation().Z, CrouchedEyeHeight, CameraCrouchSpeed);
		}
		else
		{
			NewZ = FMath::Lerp(PlayerCamera->GetRelativeLocation().Z, BaseEyeHeight, CameraCrouchSpeed);
		}

		const FVector NewLocation = FVector(CameraInitialLocation.X, CameraInitialLocation.Y, NewZ);
		PlayerCamera->SetRelativeLocation(NewLocation);
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::Jump);

	DECLARE_DELEGATE_OneParam(FCrouchDelegate, const bool);
	PlayerInputComponent->BindAction<FCrouchDelegate>("Crouch", IE_Pressed, this, &APlayerCharacter::Crouch, false);
	PlayerInputComponent->BindAction<FCrouchDelegate>("Crouch", IE_Released, this, &APlayerCharacter::UnCrouch, false);

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
	PlayerInputComponent->BindAction<FChooseWeaponDelegate>("ChooseRevolver",
	                                                        IE_Pressed,
	                                                        WeaponComponent,
	                                                        &UWeaponComponent::ChooseWeapon,
	                                                        0);
	PlayerInputComponent->BindAction<FChooseWeaponDelegate>("ChooseSMG",
	                                                        IE_Pressed,
	                                                        WeaponComponent,
	                                                        &UWeaponComponent::ChooseWeapon,
	                                                        1);
	PlayerInputComponent->BindAction<FChooseWeaponDelegate>("ChooseShotgun",
	                                                        IE_Pressed,
	                                                        WeaponComponent,
	                                                        &UWeaponComponent::ChooseWeapon,
	                                                        2);
	PlayerInputComponent->BindAction<FChooseWeaponDelegate>("ChooseAssaultRifle",
	                                                        IE_Pressed,
	                                                        WeaponComponent,
	                                                        &UWeaponComponent::ChooseWeapon,
	                                                        3);
	PlayerInputComponent->BindAction<FChooseWeaponDelegate>("ChooseSniperRifle",
	                                                        IE_Pressed,
	                                                        WeaponComponent,
	                                                        &UWeaponComponent::ChooseWeapon,
	                                                        4);
	PlayerInputComponent->BindAction<FChooseWeaponDelegate>("ChooseLauncher",
	                                                        IE_Pressed,
	                                                        WeaponComponent,
	                                                        &UWeaponComponent::ChooseWeapon,
	                                                        5);

	// Other
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::StartInteraction);
	PlayerInputComponent->BindAction("SwitchFlashlight", IE_Pressed, Flashlight, &UFlashlightComponent::Switch);
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

void APlayerCharacter::AddCameraRecoil(const float RecoilPitch, const float RecoilYaw)
{
	AddControllerPitchInput(-RecoilPitch);
	AddControllerYawInput(FMath::FRandRange(-RecoilYaw, RecoilYaw));
}

void APlayerCharacter::StartInteraction()
{
	InteractionQueue->Interact();
}
