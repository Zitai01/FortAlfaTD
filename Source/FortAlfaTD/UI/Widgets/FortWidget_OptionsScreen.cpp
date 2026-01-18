// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/FortWidget_OptionsScreen.h"
#include "FortDebugHelper.h"
#include "ICommonInputModule.h"
#include "Input/CommonUIInputTypes.h"

void UFortWidget_OptionsScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!ResetAction.IsNull())
	{
		ResetActionHandle = RegisterUIActionBinding(
			FBindUIActionArgs(
				ResetAction, true, FSimpleDelegate::CreateUObject(this, &ThisClass::OnResetBoundActionTriggered)
			)
				);
			}
	RegisterUIActionBinding(
		FBindUIActionArgs(
			ICommonInputModule::GetSettings().GetDefaultBackAction(),true,FSimpleDelegate::CreateUObject(this, &ThisClass::OnBackBoundActionTriggered)
			));
}

void UFortWidget_OptionsScreen::OnBackBoundActionTriggered()
{

	DeactivateWidget();
}

void UFortWidget_OptionsScreen::OnResetBoundActionTriggered()
{
	Debug::Print(TEXT("Reset bound action triggered"));
}
