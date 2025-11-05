// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FortResearchNodeData.generated.h"


/**
 * 
 */
UCLASS()
class FORTALFATD_API UFortResearchNodeData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName NodeID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText NodeName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Cost;
};
