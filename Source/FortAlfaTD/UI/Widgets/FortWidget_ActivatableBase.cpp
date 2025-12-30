// Fill out your copyright notice in the Description page of Project Settings.

#include "Controller/FortMenuPlayerController.h"
#include "UI/Widgets/FortWidget_ActivatableBase.h"

AFortMenuPlayerController* UFortWidget_ActivatableBase::GetOwningMenuPlayerController()
{
	if (CachedOwningFortPC.IsValid())
	{
		CachedOwningFortPC = GetOwningPlayer<AFortMenuPlayerController>();
	}

	return CachedOwningFortPC.IsValid()? CachedOwningFortPC.Get() : nullptr;
}
