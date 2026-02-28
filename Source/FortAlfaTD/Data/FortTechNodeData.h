// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "FortTechNodeData.generated.h"


class UTowerData;            
class UGameplayEffect;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FFortTechSetByCallerMod
{
	GENERATED_BODY()

	// Example: Data.Tech.AttackDamage.Add
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag DataTag;

	// Example: +5, +0.1, etc (you decide interpretation in your GE)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Magnitude = 0.f;
};

USTRUCT(BlueprintType)
struct FFortTechCost
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag CurrencyTag;     // e.g. Currency.Research.Atomic

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Amount = 0;
};

UCLASS(BlueprintType)
class FORTALFATD_API UFortTechNodeData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName NodeID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(MultiLine="true"))
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FFortTechCost> Costs;

	// Dependencies (NodeIDs)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FName> PrerequisiteNodeIDs;

	// If null => applies to ALL towers
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTowerData> TargetTowerData;

	// SetByCaller payload for your tech GE
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FFortTechSetByCallerMod> SetByCallerMods;




		
};
