// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FortEnemyBaseCharacter.h"
#include "FortEnemyBaseFlyingCharacter.generated.h"

/**
 * 
 */
UCLASS()
class FORTALFATD_API AFortEnemyBaseFlyingCharacter : public AFortEnemyBaseCharacter
{
	GENERATED_BODY()

public:
	AFortEnemyBaseFlyingCharacter();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Flying")
	float CruiseHeight = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Flying")
	float FlySpeed = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Flying")
	bool bFaceMoveDirection = true;

	FVector CurrentMoveTarget;
	
};
