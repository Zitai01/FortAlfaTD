// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Data/Types/FortEnumTypes.h"
#include "CoreMinimal.h"
#include "UI/Widgets/FortWidget_ActivatableBase.h"
#include "FortWidget_ConfirmScreen.generated.h"

class UDynamicEntryBox;
class UCommonTextBlock;

USTRUCT(BlueprintType)
struct FConfirmScreenButtonInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EConfirmScreenButtonType ConfirmScreenButtonType = EConfirmScreenButtonType::Unknown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ButtonTextToDisplay;
};

UCLASS()
class FORTALFATD_API UConfirmScreenInfoObject : public UObject
{
	GENERATED_BODY()

public:
	static UConfirmScreenInfoObject* CreateOKScreen(const FText& InScreenTitle, const FText& InScreenMessage);

	static UConfirmScreenInfoObject* CreateYesNoScreen(const FText& InScreenTitle, const FText& InScreenMessage);

	static UConfirmScreenInfoObject* CreateOKCancelScreen(const FText& InScreenTitle, const FText& InScreenMessage);

	UPROPERTY(Transient)
	FText ScreenTitle;
	
	UPROPERTY(Transient)
	FText ScreenMessage;
	
	UPROPERTY(Transient)
	TArray<FConfirmScreenButtonInfo> AvaliableScreenButtons;
};
/**
 * 
 */
UCLASS(Abstract,BlueprintType, meta = (DisableNaiveTick))
class FORTALFATD_API UFortWidget_ConfirmScreen : public UFortWidget_ActivatableBase
{
	GENERATED_BODY()

public:
	//Get Called outside the class when this widget is constructed and before it's pushed to the stack
	void InitConfirmScreen(UConfirmScreenInfoObject* InScreenInfoObject, TFunction<void(EConfirmScreenButtonType)> ClickedButtonCallback);
private:
	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* CommonTextBlock_Title;
	
	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* CommonTextBlock_Message;
	
	UPROPERTY(meta=(BindWidget))
	UDynamicEntryBox* DynamicEntryBox;
};
