// Created by Artyom Volkov during the UE4 course


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UShooterDamageControllerComponent;

UCLASS()
class ZOMBIESHOOTER_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UShooterDamageControllerComponent* DamageControllerComponent = nullptr;
};
