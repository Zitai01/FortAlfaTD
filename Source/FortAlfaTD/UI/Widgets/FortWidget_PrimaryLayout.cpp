// Fill out your copyright notice in the Description page of Project Settings.

#include "FortDebugHelper.h"
#include "UI/Widgets/FortWidget_PrimaryLayout.h"

UCommonActivatableWidgetContainerBase* UFortWidget_PrimaryLayout::FindWidgetStackByTag(const FGameplayTag& InTag) const
{
	checkf(RegisteredWidgetStackMap.Contains(InTag),TEXT("Widget with tag %s stack doesn't exist"),*InTag.ToString());
	return RegisteredWidgetStackMap.FindRef(InTag);
}

void UFortWidget_PrimaryLayout::RegisterWidgetStack(UPARAM(meta = (Categories = "FortUI.WidgetStack" ))FGameplayTag InStackTag,
                                                    UCommonActivatableWidgetContainerBase* Instack)
{
	if (!IsDesignTime())
	{
		if (!RegisteredWidgetStackMap.Contains(InStackTag))
		{
			RegisteredWidgetStackMap.Add(InStackTag, Instack);
			
		}
	}
}
