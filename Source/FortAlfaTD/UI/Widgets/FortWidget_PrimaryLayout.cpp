// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/FortWidget_PrimaryLayout.h"

#include "CommonActivatableWidget.h"
#include "Data/Types/FortDebugData.h"

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


TArray<FGameplayTag> UFortWidget_PrimaryLayout::GetRegisteredWidgetStackTags() const
{
	TArray<FGameplayTag> Keys;
	RegisteredWidgetStackMap.GetKeys(Keys);
	return Keys;
}

UCommonActivatableWidget* UFortWidget_PrimaryLayout::GetActiveWidgetInStack(FGameplayTag StackTag) const
{
	UCommonActivatableWidgetContainerBase* Container = GetRegisteredWidgetStack(StackTag);
	if (!Container) return nullptr;

	if (UCommonActivatableWidgetStack* Stack = Cast<UCommonActivatableWidgetStack>(Container))
	{
		return Stack->GetActiveWidget();
	}
	return nullptr;
}

TArray<UCommonActivatableWidget*> UFortWidget_PrimaryLayout::GetAllWidgetsInStack(FGameplayTag StackTag) const
{
	TArray<UCommonActivatableWidget*> Out;

	UCommonActivatableWidgetContainerBase* Container = GetRegisteredWidgetStack(StackTag);
	if (!Container)
	{
		return Out;
	}

	// Copy from the container's internal list into a BP-friendly array
	const TArray<UCommonActivatableWidget*>& List = Container->GetWidgetList();
	Out.Reserve(List.Num());

	for (UCommonActivatableWidget* W : List)
	{
		if (W->IsValidLowLevel())
		{
			Out.Add(W);
		}
	}

	return Out;
}

UCommonActivatableWidgetContainerBase* UFortWidget_PrimaryLayout::GetRegisteredWidgetStack(FGameplayTag StackTag) const
{
	if (UCommonActivatableWidgetContainerBase* const* Found = RegisteredWidgetStackMap.Find(StackTag))
	{
		return *Found;
	}
	return nullptr;
}

TArray<FortDebugData> UFortWidget_PrimaryLayout::GetRegisteredWidgetStacksDebug() const
{
	TArray<FortDebugData> Out;
	Out.Reserve(RegisteredWidgetStackMap.Num());

	for (const auto& KVP : RegisteredWidgetStackMap)
	{
		FortDebugData Info;
		Info.StackTag = KVP.Key;
		Info.Container = KVP.Value;
		Info.ContainerName = GetNameSafe(KVP.Value);
		Info.ContainerClass = KVP.Value ? KVP.Value->GetClass()->GetName() : TEXT("None");
		Out.Add(MoveTemp(Info));
	}
	return Out;
}