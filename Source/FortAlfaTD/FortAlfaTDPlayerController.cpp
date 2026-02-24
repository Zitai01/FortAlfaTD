// Copyright Epic Games, Inc. All Rights Reserved.

#include "FortAlfaTDPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "FortAlfaTDCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "FortAlfaTD.h"
#include "GameFramework/SpringArmComponent.h"

AFortAlfaTDPlayerController::AFortAlfaTDPlayerController()
{
	bIsTouch = false;
	bMoveToMouseCursor = false;

	// configure the controller
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void AFortAlfaTDPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Only set up input on local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}

		// Set up action bindings
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		{
			EnhancedInputComponent->BindAction(IAMove, ETriggerEvent::Triggered, this, &AFortAlfaTDPlayerController::HandleMove);
			// FortAlfaTDPlayerController.cpp
			EnhancedInputComponent->BindAction(IAZoom, ETriggerEvent::Triggered, this, &AFortAlfaTDPlayerController::HandleZoom);
			EnhancedInputComponent->BindAction(IACameraRotateHold, ETriggerEvent::Started, this, &AFortAlfaTDPlayerController::HandleCameraRotateHold);
			EnhancedInputComponent->BindAction(IACameraRotateHold, ETriggerEvent::Completed, this, &AFortAlfaTDPlayerController::HandleCameraRotateHold);
			EnhancedInputComponent->BindAction(IACameraRotateHold, ETriggerEvent::Canceled, this, &AFortAlfaTDPlayerController::HandleCameraRotateHold);
			EnhancedInputComponent->BindAction(IACameraLook, ETriggerEvent::Triggered, this, &AFortAlfaTDPlayerController::HandleCameraLook);
	//		EnhancedInputComponent->BindAction(IACameraTurn, ETriggerEvent::Triggered, this, &AFortAlfaTDPlayerController::HandleCameraTurn);
			// Setup mouse input events
			/*
			EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AFortAlfaTDPlayerController::OnInputStarted);
			EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AFortAlfaTDPlayerController::OnSetDestinationTriggered);
			EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AFortAlfaTDPlayerController::OnSetDestinationReleased);
			EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AFortAlfaTDPlayerController::OnSetDestinationReleased);
			*/

			/*
			EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &AFortAlfaTDPlayerController::OnInputStarted);
			EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &AFortAlfaTDPlayerController::OnTouchTriggered);
			EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &AFortAlfaTDPlayerController::OnTouchReleased);
			EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &AFortAlfaTDPlayerController::OnTouchReleased);
			*/
		}
		else
		{
			UE_LOG(LogFortAlfaTD, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
		}
	}
}
void AFortAlfaTDPlayerController::HandleZoom(const FInputActionValue& Value)
{
	const float Wheel = Value.Get<float>();
	if (FMath::IsNearlyZero(Wheel)) return;

	AFortAlfaTDCharacter* MyChar = Cast<AFortAlfaTDCharacter>(GetPawn());
	if (!MyChar) return;

	USpringArmComponent* Boom = MyChar->GetCameraBoom();
	if (!Boom) return;

	const float NewLen = FMath::Clamp(
		Boom->TargetArmLength - Wheel * ZoomSpeed,
		MinZoom,
		MaxZoom
	);

	Boom->TargetArmLength = NewLen;
}

void AFortAlfaTDPlayerController::OnInputStarted()
{
	StopMovement();
}

void AFortAlfaTDPlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}
	
	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void AFortAlfaTDPlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

// Triggered every frame when the input is held down
void AFortAlfaTDPlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void AFortAlfaTDPlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}

void AFortAlfaTDPlayerController::HandleMove(const FInputActionValue& Value)
{
	const FVector2D Move = Value.Get<FVector2D>();
	if (Move.IsNearlyZero()) return;

	AFortAlfaTDCharacter* MyChar = Cast<AFortAlfaTDCharacter>(GetPawn());
	if (!MyChar) return;

	USpringArmComponent* Boom = MyChar->GetCameraBoom();
	if (!Boom) return;

	// Use the camera boom yaw as the “camera facing” direction
	FRotator CamRot = Boom->GetComponentRotation();
	CamRot.Pitch = 0.f;
	CamRot.Roll  = 0.f;

	const FVector Forward = FRotationMatrix(CamRot).GetUnitAxis(EAxis::X);
	const FVector Right   = FRotationMatrix(CamRot).GetUnitAxis(EAxis::Y);

	// Typical WASD: Y is forward/back, X is right/left (depends on how your IA is set)
	MyChar->AddMovementInput(Forward, Move.Y);
	MyChar->AddMovementInput(Right,   Move.X);
}
void AFortAlfaTDPlayerController::HandleCameraRotateHold(const FInputActionValue& Value)
{
	bCameraRotating = Value.Get<bool>();

	// Optional feel improvements:
	// bShowMouseCursor = !bCameraRotating;
	// SetIgnoreLookInput(bCameraRotating);
}

void AFortAlfaTDPlayerController::HandleCameraLook(const FInputActionValue& Value)
{
	if (!bCameraRotating) return;

	const FVector2D Look = Value.Get<FVector2D>();
	if (Look.IsNearlyZero()) return;

	AFortAlfaTDCharacter* MyChar = Cast<AFortAlfaTDCharacter>(GetPawn());
	if (!MyChar) return;

	USpringArmComponent* Boom = MyChar->GetCameraBoom();
	if (!Boom) return;

	const float Dt = GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.f;

	FRotator R = Boom->GetComponentRotation(); // use world rotation (absolute boom)

	// Yaw from Mouse X
	R.Yaw += Look.X * CameraYawSpeed * Dt;

	// Pitch from Mouse Y (invert if needed)
	R.Pitch = FMath::Clamp(
		R.Pitch + (Look.Y * CameraPitchSpeed * Dt),
		MinPitch,
		MaxPitch
	);

	R.Roll = 0.f;

	Boom->SetWorldRotation(R);
}