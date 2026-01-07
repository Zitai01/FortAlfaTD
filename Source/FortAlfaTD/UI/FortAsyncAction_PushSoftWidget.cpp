// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FortAsyncAction_PushSoftWidget.h"
#include "FortUISubsystem.h"
#include "UI/Widgets/FortWidget_ActivatableBase.h"
UFortAsyncAction_PushSoftWidget* UFortAsyncAction_PushSoftWidget::PushSoftWidget(const UObject* WorldContextObject,
                                                                                 APlayerController* OwningPlayerController, TSoftClassPtr<UFortWidget_ActivatableBase> InSoftWidgetClass,
                                                                                 UPARAM(meta = (Categories = "FortUI.WidgetStack" )) FGameplayTag InWidgetStackTag, bool bFocusOnNewlyPushedWidget)
{
	checkf(!InSoftWidgetClass.IsNull(),TEXT("PushSoftWidgetToStack was passed a null soft widget class"));

	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject,EGetWorldErrorMode::LogAndReturnNull))
		{
			UFortAsyncAction_PushSoftWidget* Node = NewObject<UFortAsyncAction_PushSoftWidget>();
			Node->CachedOwningWorld = World;
			Node->CachedPlayerController = OwningPlayerController;
			Node->CachedSoftWidgetClass = InSoftWidgetClass;
			Node->CachedWidgetStackTag = InWidgetStackTag;
			Node->RegisterWithGameInstance(World);

			return Node;
		}
	}
	return nullptr;
}

void UFortAsyncAction_PushSoftWidget::Activate()
{
	Super::Activate();
	UFortUISubsystem* FortUISubsystem =  UFortUISubsystem::Get(CachedOwningWorld.Get());

	FortUISubsystem->PushSoftWidgetToStackAsync(CachedWidgetStackTag,CachedSoftWidgetClass,
		[this](EAsyncPushWidgetState InPushState, UFortWidget_ActivatableBase* PushedWidget)
		{
			switch (InPushState)
			{
			case EAsyncPushWidgetState::OnCreatedBeforePush:
				PushedWidget->SetOwningPlayer(CachedPlayerController.Get());

				OnWidgetCreatedBeforePush.Broadcast(PushedWidget);
				
				break;
				
			case EAsyncPushWidgetState::AfterPush:

				AfterPush.Broadcast(PushedWidget);

				if (bCachedFocusOnNewlyPushedWidget)
				{
					if (UWidget* WidgetToFocus = PushedWidget->GetDesiredFocusTarget())
					{
						WidgetToFocus->SetFocus();
					}
				}

				SetReadyToDestroy();
				break;
			default:
				break;
			}
		});
}
