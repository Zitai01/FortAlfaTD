// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScalableFloat.h"
#include "Engine/DataAsset.h"
#include "TowerData.generated.h"

/**
 * 
 */
UCLASS()
class FORTALFATD_API UTowerData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tower")
	FString TowerName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tower")
	float Damage = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tower")
	FScalableFloat DamageMultiplier = 1.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tower")
	float FireRate = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tower")
	float Range = 800.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tower")
	float AbilityDamage = 10.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tower")
	int32 BuildCost = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tower")
	int32 ProjectTileSpeed = 1000;
};
