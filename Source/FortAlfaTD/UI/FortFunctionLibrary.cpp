// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FortFunctionLibrary.h"

#include "DeveloperSettings/FortDeveloperSettings.h"

TSoftClassPtr<UFortWidget_ActivatableBase> UFortFunctionLibrary::GetFortSoftWidgetClassByTag(UPARAM(meta = (Catagories = "FortUI.Widget"))FGameplayTag InWidgetTag)
{
	const UFortDeveloperSettings* FortDeveloperSettings = GetDefault<UFortDeveloperSettings>();
	checkf(FortDeveloperSettings->FrontendWidgetMap.Contains(InWidgetTag),TEXT("Could not find the corresponding widget under the tag %s"),*InWidgetTag.ToString());

	return FortDeveloperSettings->FrontendWidgetMap.FindRef(InWidgetTag);
}
