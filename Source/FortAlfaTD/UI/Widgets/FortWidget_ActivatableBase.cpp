// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Widgets/FortWidget_ActivatableBase.h"
#include "Controller/FortMenuPlayerController.h"


AFortMenuPlayerController* UFortWidget_ActivatableBase::GetOwningMenuPlayerController()
{
	if (!CachedOwningFortPC.IsValid())
	{
		CachedOwningFortPC = GetOwningPlayer<AFortMenuPlayerController>();
	}

	return CachedOwningFortPC.IsValid()? CachedOwningFortPC.Get() : nullptr;
}
