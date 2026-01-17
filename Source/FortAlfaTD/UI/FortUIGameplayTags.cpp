// Fill out your copyright notice in the Description page of Project Settings.

#include "FortUIGameplayTags.h"

namespace FortUI
{
	namespace Widget
	{
		UE_DEFINE_GAMEPLAY_TAG(PressAnyKeyScreen,"FortUI.Widget.PressAnyKeyScreen");
		UE_DEFINE_GAMEPLAY_TAG(MainMenuScreen,"FortUI.Widget.MainMenuScreen");
		UE_DEFINE_GAMEPLAY_TAG(ConfirmScreen,"FortUI.Widget.ConfirmScreen");
		
	}

	namespace WidgetStack
	{
		UE_DEFINE_GAMEPLAY_TAG(Modal,"FortUI.WidgetStack.Modal");
		UE_DEFINE_GAMEPLAY_TAG(GameMenu,"FortUI.WidgetStack.GameMenu");
		UE_DEFINE_GAMEPLAY_TAG(GameHud,"FortUI.WidgetStack.GameHud");
		UE_DEFINE_GAMEPLAY_TAG(Frontend,"FortUI.WidgetStack.Frontend");
	}
	
}

namespace State
{
	namespace Firing
	{
		UE_DEFINE_GAMEPLAY_TAG(Laser,"State.Firing.Laser");
	}
}