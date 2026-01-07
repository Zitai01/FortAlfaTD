#pragma once
#include "GameplayTagContainer.h"
#include "FortDebugData.generated.h" 
class UCommonActivatableWidgetContainerBase;

USTRUCT(BlueprintType)
struct FORTALFATD_API FortDebugData
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag StackTag;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCommonActivatableWidgetContainerBase> Container = nullptr;

	UPROPERTY(BlueprintReadOnly)
	FString ContainerName;

	UPROPERTY(BlueprintReadOnly)
	FString ContainerClass;
};

