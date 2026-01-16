// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "FortWidget_PrimaryLayout.generated.h"

struct FortDebugData;
/**
 * 
 */
class UCommonActivatableWidgetContainerBase;
/**
 * 
 */
UCLASS(Abstract,BlueprintType, meta = (DisableNativeTick))
class FORTALFATD_API UFortWidget_PrimaryLayout : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UCommonActivatableWidgetContainerBase* FindWidgetStackByTag(const FGameplayTag& InTag) const;
	
	UFUNCTION(BlueprintPure, Category="UI|Stacks")
	TArray<FortDebugData> GetRegisteredWidgetStacksDebug() const;

	UFUNCTION(BlueprintPure, Category="UI|Stacks")
	UCommonActivatableWidgetContainerBase* GetRegisteredWidgetStack(FGameplayTag StackTag) const;

	UFUNCTION(BlueprintPure, Category="UI|Stacks")
	TArray<FGameplayTag> GetRegisteredWidgetStackTags() const;

	UFUNCTION(BlueprintPure, Category="UI|Stacks")
	UCommonActivatableWidget* GetActiveWidgetInStack(FGameplayTag StackTag) const;

	UFUNCTION(BlueprintPure, Category="UI|Stacks")
	TArray<UCommonActivatableWidget*> GetAllWidgetsInStack(FGameplayTag StackTag) const;

protected:
	UFUNCTION(BlueprintCallable)
	void RegisterWidgetStack(UPARAM(meta = (Categories = "FortUI.WidgetStack" )) FGameplayTag InStackTag, UCommonActivatableWidgetContainerBase* Instack);

private:
	UPROPERTY(Transient)
	TMap<FGameplayTag,UCommonActivatableWidgetContainerBase*> RegisteredWidgetStackMap;
};
