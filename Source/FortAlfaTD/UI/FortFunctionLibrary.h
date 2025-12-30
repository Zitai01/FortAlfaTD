// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FortFunctionLibrary.generated.h"

class UFortWidget_ActivatableBase;
/**
 * 
 */
UCLASS()
class FORTALFATD_API UFortFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fort Function Library")
	static TSoftClassPtr<UFortWidget_ActivatableBase> GetFortSoftWidgetClassByTag(UPARAM(meta = (Catagories = "FortUI.Widget"))FGameplayTag InWidgetTag);
};
