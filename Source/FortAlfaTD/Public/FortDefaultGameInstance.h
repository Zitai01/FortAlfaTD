// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FortDefaultGameInstance.generated.h"

class UFortWaveData;
/**
 * 
 */
UCLASS()
class FORTALFATD_API UFortDefaultGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FName> MapPool;

	UPROPERTY(BlueprintReadWrite)
	int32 CurrentMapIndex = 0;

	UPROPERTY(BlueprintReadWrite)
	int32 SessionNumber = 0;

	UPROPERTY(BlueprintReadWrite)
	int32 Mineral = 100;
	
	UPROPERTY(BlueprintReadWrite)
	int32 CurrentLevel = 1;
	
	UPROPERTY(BlueprintReadWrite)
	int32 Currency = 0;
	
	UPROPERTY(BlueprintReadWrite)
	int32 AntiMatterPartical = 0;

	UPROPERTY(BlueprintReadWrite)
	bool InSession = false;
	
	UFUNCTION(BlueprintCallable)
	void StartSession();
	
	UFUNCTION(BlueprintCallable)
	void OnSessionComplete(bool bSuccess);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Mission")
	TArray<TSoftObjectPtr<UFortWaveData>> SelectedWaves;
};
