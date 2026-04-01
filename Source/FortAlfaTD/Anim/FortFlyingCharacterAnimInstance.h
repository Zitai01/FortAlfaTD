// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FortFlyingCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FORTALFATD_API UFortFlyingCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category="Animation")
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category="Animation")
	float VerticalSpeed;

	UPROPERTY(BlueprintReadOnly, Category="Animation")
	bool bIsGrounded;

	UPROPERTY(BlueprintReadOnly, Category="Animation")
	bool bIsFalling;

	UPROPERTY(BlueprintReadOnly, Category="Animation")
	bool bIsFlying;

protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
};
