// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FortTechTreeData.generated.h"

class UFortTechNodeData;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FFortTechNodeUIEntry
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UFortTechNodeData> Node = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector2D UIPosition = FVector2D::ZeroVector;
};

UCLASS(BlueprintType)
class FORTALFATD_API UFortTechTreeData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName TreeID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FFortTechNodeUIEntry> Nodes;

	// Optional: nodes that start unlocked for free
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FName> StartingUnlockedNodeIDs;
};
