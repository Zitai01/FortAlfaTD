// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/FortWaveData.h"
#include "Engine/DataAsset.h"
#include "FortMissionWaveSet.generated.h"

/**
 * 
 */
UCLASS()
class FORTALFATD_API UFortMissionWaveSet : public UPrimaryDataAsset
{
	GENERATED_BODY()

	
public:
	// Mission number (or enum)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MissionId = 0;

	// Different difficulty wave lists
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UFortWaveData*> Waves_Easy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UFortWaveData*> Waves_Normal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UFortWaveData*> Waves_Hard;
};
