// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "FortAsyncAction_PushSoftWidget.generated.h"
class UFortWidget_ActivatableBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPushSoftWidgetDelegate, UFortWidget_ActivatableBase*, PushedWidget);
/**
 * 
 */
UCLASS()
class FORTALFATD_API UFortAsyncAction_PushSoftWidget : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", HidePin = "WorldContextObject",BlueprintInternalUseOnly = "true",DisplayName = "Push Soft Widget To Widget Stack"))
	static UFortAsyncAction_PushSoftWidget* PushSoftWidget(const UObject* WorldContextObject,
		APlayerController* OwningPlayerController,
		TSoftClassPtr<UFortWidget_ActivatableBase> InSoftWidgetClass,
		UPARAM(meta = (Categories = "FortUI.WidgetStack" )) FGameplayTag InWidgetStackTag,
		bool bFocusOnNewlyPushedWidget = true);

	UPROPERTY(Blueprintassignable)
	FOnPushSoftWidgetDelegate OnWidgetCreatedBeforePush;

	UPROPERTY(Blueprintassignable)
	FOnPushSoftWidgetDelegate AfterPush;

	virtual void Activate() override;
	
	private:
	TWeakObjectPtr<UWorld> CachedOwningWorld;
	TWeakObjectPtr<APlayerController> CachedPlayerController;
	TSoftClassPtr<UFortWidget_ActivatableBase> CachedSoftWidgetClass;
	FGameplayTag CachedWidgetStackTag;
	bool bCachedFocusOnNewlyPushedWidget = false;
};
