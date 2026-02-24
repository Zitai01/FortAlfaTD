// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "FortAlfaTDPlayerController.generated.h"

struct FInputActionValue;
class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  Player controller for a top-down perspective game.
 *  Implements point and click based controls
 */
UCLASS(abstract)
class AFortAlfaTDPlayerController : public APlayerController
{
	GENERATED_BODY()
private:
	bool bCameraRotating = false;

	void HandleCameraRotateHold(const FInputActionValue& Value);
	void HandleCameraLook(const FInputActionValue& Value);
protected:

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, Category="Input")
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UNiagaraSystem* FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputMappingContext* DefaultMappingContext;
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* SetDestinationClickAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* SetDestinationTouchAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Camera")
	float ZoomSpeed = 300.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Camera")
	float MinZoom = 500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Camera")
	float MaxZoom = 6000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<const UInputAction> IACameraRotateHold;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<const UInputAction> IACameraTurn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<const UInputAction> IACameraLook;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Camera")
	float CameraYawSpeed = 180.f; // degrees/sec-ish feel (tune)
	
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	/** Set to true if we're using touch input */
	uint32 bIsTouch : 1;

	/** Saved location of the character movement destination */
	FVector CachedDestination;

	/** Time that the click input has been pressed */
	float FollowTime = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category="Camera")
	float CameraPitchSpeed = 120.f;

	UPROPERTY(EditDefaultsOnly, Category="Camera")
	float MinPitch = -80.f;

	UPROPERTY(EditDefaultsOnly, Category="Camera")
	float MaxPitch = -15.f;
public:

	/** Constructor */
	AFortAlfaTDPlayerController();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<const UInputAction> IAMove;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<const UInputAction> IAZoom;
	/** Initialize input bindings */
	virtual void SetupInputComponent() override;
	/** Input handlers */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
	void OnTouchTriggered();
	void OnTouchReleased();
	void HandleMove(const FInputActionValue& Value);
	void HandleZoom(const FInputActionValue& Value);
};


