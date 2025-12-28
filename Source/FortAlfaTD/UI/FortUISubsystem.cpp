// Fill out your copyright notice in the Description page of Project Settings.

#include "FortDebugHelper.h"
#include "UI/FortUISubsystem.h"
#include "Engine/AssetManager.h"
#include "Widgets/FortWidget_ActivatableBase.h"
#include "Widgets/FortWidget_PrimaryLayout.h"

UFortUISubsystem* UFortUISubsystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject,EGetWorldErrorMode::Assert);
		return UGameInstance::GetSubsystem<UFortUISubsystem>(World->GetGameInstance());
	}
	return nullptr;
}

bool UFortUISubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(),FoundClasses);
		return FoundClasses.IsEmpty();
	}
	
	return false;
}

void UFortUISubsystem::RegisterCreatedPrimaryLayoutWidget(UFortWidget_PrimaryLayout* InCreatedWidget)
{
	check(InCreatedWidget);

	CreatedPrimaryLayout = InCreatedWidget;

	Debug::Print(TEXT("PrimaryLayoutWidget stored"));
}

void UFortUISubsystem::PushSoftWidgetToStackAsync(const FGameplayTag& InWidgetStackTag,
	TSoftClassPtr<UFortWidget_ActivatableBase> InSoftWidgetClass, TFunction<void(EAsyncPushWidgetState,UFortWidget_ActivatableBase*)> AysncPushStateCallback)
{
	check(InSoftWidgetClass.IsNull());

	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
		InSoftWidgetClass.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[InSoftWidgetClass,this,InWidgetStackTag,AysncPushStateCallback]()
			{
				UClass* LoadedWidgetClass = InSoftWidgetClass.Get();
				check(LoadedWidgetClass);
				UCommonActivatableWidgetContainerBase* FoundWidgetStack = CreatedPrimaryLayout->FindWidgetStackByTag(InWidgetStackTag);

				UFortWidget_ActivatableBase* CreatedWidget =  FoundWidgetStack->AddWidget<UFortWidget_ActivatableBase>(
					LoadedWidgetClass,
					[AysncPushStateCallback](UFortWidget_ActivatableBase& CreatedWidgetInstance)
					{
						AysncPushStateCallback(EAsyncPushWidgetState::OnCreatedBeforePush, &CreatedWidgetInstance);
					}
				);

				AysncPushStateCallback(EAsyncPushWidgetState::AfterPush, CreatedWidget);
				
			}
			));
}	
