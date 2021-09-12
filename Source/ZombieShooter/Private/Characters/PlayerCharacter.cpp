// Created by Artyom Volkov during the UE4 course


#include "Characters/PlayerCharacter.h"
#include "Camera/CameraComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>("PlayerCamera");
	PlayerCamera->SetupAttachment(GetRootComponent());

	PlayerArms = CreateDefaultSubobject<USkeletalMeshComponent>("PlayerArms");
	PlayerArms->SetupAttachment(PlayerCamera);
	PlayerArms->CastShadow = false;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
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

	// Weapon
	PlayerInputComponent->BindAction("EquipNextWeapon",
	                                 IE_Pressed,
	                                 WeaponComponent,
	                                 &UWeaponComponent::EquipNextWeapon);
	PlayerInputComponent->BindAction("EquipPreviousWeapon",
	                                 IE_Pressed,
	                                 WeaponComponent,
	                                 &UWeaponComponent::EquipPreviousWeapon);
}

void APlayerCharacter::MoveForward(const float AxisValue)
{
	AddMovementInput(GetActorForwardVector(), AxisValue);
}

void APlayerCharacter::MoveRight(const float AxisValue)
{
	AddMovementInput(GetActorRightVector(), AxisValue);
}
