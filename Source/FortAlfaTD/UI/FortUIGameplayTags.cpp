// Fill out your copyright notice in the Description page of Project Settings.

#include "FortUIGameplayTags.h"

namespace FortUIGameplayTags
{
	//Fort Widget Stack
	UE_DEFINE_GAMEPLAY_TAG(FortUI_WidgetStack_Modal,"FortUI.WidgetStack.Modal");
	UE_DEFINE_GAMEPLAY_TAG(FortUI_WidgetStack_GameMenu,"FortUI.WidgetStack.GameMenu");
	UE_DEFINE_GAMEPLAY_TAG(FortUI_WidgetStack_GameHud,"FortUI.WidgetStack.GameHud");
	UE_DEFINE_GAMEPLAY_TAG(FortUI_WidgetStack_Frontend,"FortUI.WidgetStack.Frontend");

	//Fort Widgets
	UE_DEFINE_GAMEPLAY_TAG(FortUI_Widget_PressAnyKeyScreen,"FortUI.Widget.PressAnyKeyScreen");
	UE_DEFINE_GAMEPLAY_TAG(FortUI_Widget_MainMenuScreen,"FortUI.Widget.MainMenuScreen");
	UE_DEFINE_GAMEPLAY_TAG(FortUI_Widget_ConfirmScreen,"FortUI.Widget.ConfirmScreen");
}