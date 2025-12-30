// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "FortDeveloperSettings.generated.h"

class UFortWidget_ActivatableBase;
/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Fort UI Setting"))
class FORTALFATD_API UFortDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, Category = "Widget Reference", meta = (ForceInlineRow,Catagory = "FortUI.Widget"))
	TMap<FGameplayTag,TSoftClassPtr<UFortWidget_ActivatableBase>> FrontendWidgetMap;
};
