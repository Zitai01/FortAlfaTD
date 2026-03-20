// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FortEnemyBaseCharacter.h"
#include "FortEnemyBaseFlyingCharacter.generated.h"

class UProjectileMovementComponent;
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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flying")
	float FlightSpeed = 800.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flying")
	float SteeringInterval = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flying")
	bool bAddOrganicWobble = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flying", meta = (EditCondition = "bAddOrganicWobble"))
	float WobbleStrength = 0.15f;

	UPROPERTY(BlueprintReadOnly, Category = "Flying")
	TObjectPtr<AActor> MoveTargetActor = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Flying")
	bool bIsFlying = true;

	FTimerHandle SteeringTimerHandle;

	UFUNCTION()
	void UpdateFlightSteering();

public:
	UFUNCTION(BlueprintCallable, Category = "Flying")
	void SetMoveTarget(AActor* NewTarget);

	UFUNCTION(BlueprintPure, Category = "Flying")
	AActor* GetMoveTarget() const { return MoveTargetActor; }

	UFUNCTION(BlueprintPure, Category = "Flying")
	bool IsFlying() const { return bIsFlying; }
};
