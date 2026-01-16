// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"

#include "FortCommonButtonBase.generated.h"

class UCommonTextBlock;
/**
 * 
 */
UCLASS(Abstract,BlueprintType, meta = (DisableNativeTick))
class FORTALFATD_API UFortCommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

	public:
	UFUNCTION(BlueprintCallable, Category = Common)
	void SetButtonText(FText InText);
private:
	// Begin UUserWidget Interface
	virtual void NativePreConstruct() override;
	// End UUserWidget Interface
	// Begin UCommonButtonBase Interface
	virtual void NativeOnCurrentTextStyleChanged();
	virtual void NativeOnHovered();
	virtual void NativeOnUnhovered();
	// End UCommonButtonBase Interface
	
	UPROPERTY(meta=(BindWidgetOptional))
	UCommonTextBlock* CommonTextBlock_ButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fort Button", meta = (AllowPrivateAccess = "true"))
	FText ButtonDisplayText;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fort Button", meta = (AllowPrivateAccess = "true"))
	bool bUseUpperCaseForButtonText = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fort Button", meta = (AllowPrivateAccess = "true"))
	FText ButtonDescriptionText;
};
