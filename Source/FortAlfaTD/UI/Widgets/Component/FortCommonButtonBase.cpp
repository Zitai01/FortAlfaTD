// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/Component/FortCommonButtonBase.h"
#include "CommonTextBlock.h"
#include "UI/FortUISubsystem.h"

void UFortCommonButtonBase::SetButtonText(FText InText)
{
	if (CommonTextBlock_ButtonText && !InText.IsEmpty())
	{
		CommonTextBlock_ButtonText->SetText(bUseUpperCaseForButtonText? InText.ToUpper() : InText);
	}
}

void UFortCommonButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetButtonText(ButtonDisplayText);
	
}

void UFortCommonButtonBase::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged();

	if (CommonTextBlock_ButtonText && GetCurrentTextStyleClass())
	{
		CommonTextBlock_ButtonText->SetStyle(GetCurrentTextStyleClass());
	}
}

void UFortCommonButtonBase::NativeOnHovered()
{
	Super::NativeOnHovered();

	if (!ButtonDescriptionText.IsEmpty())
	{
		UFortUISubsystem::Get(this)->OnButtonDescriptionTextUpdated.Broadcast(this,ButtonDescriptionText);
	}
}

void UFortCommonButtonBase::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();
	UFortUISubsystem::Get(this)->OnButtonDescriptionTextUpdated.Broadcast(this,FText::GetEmpty());
}
