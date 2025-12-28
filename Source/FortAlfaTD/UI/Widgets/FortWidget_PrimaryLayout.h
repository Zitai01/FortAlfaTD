// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "FortWidget_PrimaryLayout.generated.h"

/**
 * 
 */
class UCommonActivatableWidgetContainerBase;
/**
 * 
 */
UCLASS(Abstract,BlueprintType, meta = (DisableNaiveTick))
class FORTALFATD_API UFortWidget_PrimaryLayout : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UCommonActivatableWidgetContainerBase* FindWidgetStackByTag(const FGameplayTag& InTag) const;
	
protected:
	UFUNCTION(BlueprintCallable)
	void RegisterWidgetStack(UPARAM(meta = (Categories = "FortUI.WidgetStack" )) FGameplayTag InStackTag, UCommonActivatableWidgetContainerBase* Instack);

private:
	UPROPERTY(Transient)
	TMap<FGameplayTag,UCommonActivatableWidgetContainerBase*> RegisteredWidgetStackMap;
};
