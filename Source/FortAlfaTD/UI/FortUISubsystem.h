// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Widgets/FortWidget_ActivatableBase.h"
#include "FortUISubsystem.generated.h"

class UFortCommonButtonBase;
class UFortWidget_PrimaryLayout;
class UFortWidget_ActivatableBase;
struct FGameplayTag;

enum class EAsyncPushWidgetState : uint8
{
	OnCreatedBeforePush,
	AfterPush
	};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnButtonDescriptionTextUpdatedDelegate,UFortCommonButtonBase*,BroadCastingButton,FText,DescriptionText);

UCLASS()
class FORTALFATD_API UFortUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UFortUISubsystem* Get(const UObject* WorldContextObject);

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	UFUNCTION(BlueprintCallable)
	void RegisterCreatedPrimaryLayoutWidget(UFortWidget_PrimaryLayout* InCreatedWidget);

    void PushSoftWidgetToStackAsync(const FGameplayTag& InWidgetStackTag,TSoftClassPtr<UFortWidget_ActivatableBase> InSoftWidgetClass, TFunction<void(EAsyncPushWidgetState,UFortWidget_ActivatableBase*)> AysncPushStateCallback);

	UPROPERTY(BlueprintAssignable)
	FOnButtonDescriptionTextUpdatedDelegate OnButtonDescriptionTextUpdated;
	
private:
	UPROPERTY(Transient)
	UFortWidget_PrimaryLayout* CreatedPrimaryLayout;
};
