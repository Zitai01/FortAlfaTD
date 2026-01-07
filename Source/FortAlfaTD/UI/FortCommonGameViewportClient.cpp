// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FortCommonGameViewportClient.h"

UFortCommonGameViewportClient::UFortCommonGameViewportClient()
	:Super()
{
	OnRerouteInput().BindUObject(this, &UCommonGameViewportClient::HandleRerouteInput);
	OnRerouteAxis().BindUObject(this, &UCommonGameViewportClient::HandleRerouteAxis);
	OnRerouteTouch().BindUObject(this, &UCommonGameViewportClient::HandleRerouteTouch);
}
