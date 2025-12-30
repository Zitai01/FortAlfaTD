// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/Component/FortCommonButtonBase.h"
#include "CommonTextBlock.h"

void UFortCommonButtonBase::SetButtonText(FText InText)
{
	if (CommonTextBlock_ButtonText && !InText.IsEmpty())
	{
		CommonTextBlock_ButtonText->SetText(bUseUpperCaseForButtonText? InText.ToUpper() : InText.ToLower());
	}
}

void UFortCommonButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetButtonText(ButtonDisplayText);
	
}
