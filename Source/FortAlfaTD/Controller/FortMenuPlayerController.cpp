// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/FortMenuPlayerController.h"

#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"

void AFortMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> FoundCameras;
	UGameplayStatics::GetAllActorsOfClassWithTag(this, ACameraActor::StaticClass(),FName("Default"),FoundCameras);

	if (!FoundCameras.IsEmpty())
	{
		SetViewTarget(FoundCameras[0]);
	}
}

