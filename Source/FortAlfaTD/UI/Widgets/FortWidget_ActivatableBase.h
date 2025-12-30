// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "FortWidget_ActivatableBase.generated.h"

class AFortMenuPlayerController;
/**
 * 
 */
UCLASS(Abstract,BlueprintType, meta = (DisableNaiveTick))
class FORTALFATD_API UFortWidget_ActivatableBase : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintPure)
	AFortMenuPlayerController* GetOwningMenuPlayerController();
private:
	TWeakObjectPtr<AFortMenuPlayerController> CachedOwningFortPC;
};
