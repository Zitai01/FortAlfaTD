// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FortWaveData.generated.h"

USTRUCT(BlueprintType)
struct FFortEnemySpawnInfo
{
	GENERATED_BODY()

public:

	// Which enemy to spawn
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawn")
	TSubclassOf<APawn> EnemyClass;

	// Number of enemies in this group
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawn")
	int32 Count = 5;

	// Delay between each enemy spawn
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawn")
	float SpawnInterval = 1.0f;

	// Which lane to use (lane = group of spawn points)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawn")
	int32 LaneIndex = 0;
};

/**
 * 
 */
UCLASS(BlueprintType)
class FORTALFATD_API UFortWaveData : public UPrimaryDataAsset
{
	GENERATED_BODY()


public:

	// List of spawn groups (executed in order)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Wave", meta=(TitleProperty="EnemyClass"))
	TArray<FFortEnemySpawnInfo> SpawnGroups;

	// Optional delay before the next wave begins
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Wave")
	float DelayBeforeNextWave = 5.0f;
};
