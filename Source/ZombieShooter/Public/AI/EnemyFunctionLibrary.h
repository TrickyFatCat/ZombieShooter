// Created by Artyom Volkov during the UE4 course

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EnemyFunctionLibrary.generated.h"

class AEnemyCharacter;

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API UEnemyFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Enemy")
	static void AggroEnemies(TArray<AEnemyCharacter*> Enemies);

	UFUNCTION(BlueprintCallable, Category="Enemy")
	static void AggroEnemy(AEnemyCharacter* Enemy);
};
