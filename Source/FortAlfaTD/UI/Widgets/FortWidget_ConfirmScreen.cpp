// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/FortWidget_ConfirmScreen.h"
#include "ICommonInputModule.h"
#include "CommonTextBlock.h"
#include "Component/FortCommonButtonBase.h"
#include "Components/DynamicEntryBox.h"

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOKScreen(const FText& InScreenTitle,
                                                                   const FText& InScreenMessage)
{
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMessage;

	FConfirmScreenButtonInfo OKButtonInfo;
	OKButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Closed;
	OKButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("OK"));

	InfoObject->AvaliableScreenButtons.Add(OKButtonInfo);
	return InfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateYesNoScreen(const FText& InScreenTitle,
	const FText& InScreenMessage)
{
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMessage;

	FConfirmScreenButtonInfo YesButtonInfo;
	YesButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Comfirmed;
	YesButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("Yes"));
	
	FConfirmScreenButtonInfo NoButtonInfo;
	NoButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Cancelled;
	NoButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("No"));

	InfoObject->AvaliableScreenButtons.Add(YesButtonInfo);
	InfoObject->AvaliableScreenButtons.Add(NoButtonInfo);
	return InfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOKCancelScreen(const FText& InScreenTitle,
	const FText& InScreenMessage)
{
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMessage;

	FConfirmScreenButtonInfo OKButtonInfo;
	OKButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Comfirmed;
	OKButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("OK"));
	
	FConfirmScreenButtonInfo CancelButtonInfo;
	CancelButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Cancelled;
	CancelButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("Cancel"));

	InfoObject->AvaliableScreenButtons.Add(OKButtonInfo);
	InfoObject->AvaliableScreenButtons.Add(CancelButtonInfo);
	return InfoObject;
}

void UFortWidget_ConfirmScreen::InitConfirmScreen(UConfirmScreenInfoObject* InScreenInfoObject,
	TFunction<void(EConfirmScreenButtonType)> ClickedButtonCallback)
{
	check(InScreenInfoObject && CommonTextBlock_Title && CommonTextBlock_Message && DynamicEntryBox);

	CommonTextBlock_Title->SetText(InScreenInfoObject->ScreenTitle);
	CommonTextBlock_Message->SetText(InScreenInfoObject->ScreenMessage);

	//If entry box has old button created previously
	if (DynamicEntryBox->GetNumEntries() != 0)
	{
		//Widget type for the entry box is specified in the child widget blueprint.
		DynamicEntryBox->Reset<UFortCommonButtonBase>(
			[](UFortCommonButtonBase& ExistingButton)
			{
				ExistingButton.OnClicked().Clear();
			});
	}

	check(!InScreenInfoObject->AvaliableScreenButtons.IsEmpty());

	for (const FConfirmScreenButtonInfo& AvaliableButtonInfo : InScreenInfoObject->AvaliableScreenButtons)
	{
		FDataTableRowHandle InputActionRowHandle;
		switch (AvaliableButtonInfo.ConfirmScreenButtonType)
		{
		case EConfirmScreenButtonType::Comfirmed:
			InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultClickAction();
			break;
		case EConfirmScreenButtonType::Cancelled:
			InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultClickAction();
			default:
			break;
		}
		UFortCommonButtonBase* AddedButton =  DynamicEntryBox->CreateEntry<UFortCommonButtonBase>();
		AddedButton->SetButtonText(AvaliableButtonInfo.ButtonTextToDisplay);
		AddedButton->SetTriggeringInputAction(InputActionRowHandle);
		AddedButton->OnClicked().AddLambda(
			[ClickedButtonCallback,AvaliableButtonInfo,this]()
			{
				ClickedButtonCallback(AvaliableButtonInfo.ConfirmScreenButtonType);
				DeactivateWidget();
			});
	}
	if (DynamicEntryBox->GetNumEntries() != 0)
	{
		//Set focus on the last button so if there are two buttons, gamepad will focus on the no button
		//DynamicEntryBox->GetAllEntries().Last()->SetFocus();
	}
}
