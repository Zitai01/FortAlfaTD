// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FortWaveEntry.generated.h"
class UFortEnemyDataAsset;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FORTALFATD_API FortWaveEntry
{
	GENERATED_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UFortEnemyDataAsset* EnemyData;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Count = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnInterval = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Wave")
	int32 LaneIndex = 0;
};
