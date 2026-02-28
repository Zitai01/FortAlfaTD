// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

namespace FortUI
{
	namespace Widget
	{
		//Fort Widget
		FORTALFATD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(PressAnyKeyScreen);
		FORTALFATD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MainMenuScreen);
		FORTALFATD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(OptionScreen);
		FORTALFATD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ConfirmScreen);
	}

	namespace WidgetStack
	{
		//Fort Widget Stack
		FORTALFATD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Modal);
		FORTALFATD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameMenu);
		FORTALFATD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameHud);
		FORTALFATD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend);
	}
}

namespace State
{
	namespace Firing
	{
		FORTALFATD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Laser);
	}
}

namespace GameplayCue
{
	FORTALFATD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(HitFlash);
}

namespace Currency
{
	namespace Research
	{
		FORTALFATD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Atomic);
		FORTALFATD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Photon);
	}
}