// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/FortWidget_ActivatableBase.h"
#include "FortWidget_OptionsScreen.generated.h"

/**
 * 
 */
UCLASS(Abstract,BlueprintType, meta = (DisableNativeTick))
class FORTALFATD_API UFortWidget_OptionsScreen : public UFortWidget_ActivatableBase
{
	GENERATED_BODY()

protected:
	// UUserWidget Interface
	virtual void NativeOnInitialized() override;
	
private:
	void OnBackBoundActionTriggered();
	void OnResetBoundActionTriggered();

	FUIActionBindingHandle ResetActionHandle;
	
	UPROPERTY(EditDefaultsOnly, Category = "FortUI Option Screen", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ResetAction;
};
