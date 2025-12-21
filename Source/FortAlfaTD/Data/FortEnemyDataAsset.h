// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "FortEnemyDataAsset.generated.h"

class UGameplayEffect;
class AFortEnemyBaseCharacter;
/**
 * 
 */
UCLASS()
class FORTALFATD_API UFortEnemyDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AFortEnemyBaseCharacter> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Armor;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> StartupEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer EnemyTags;
};
