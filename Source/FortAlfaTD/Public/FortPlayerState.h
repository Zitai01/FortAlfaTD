// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "FortPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class FORTALFATD_API AFortPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AFortPlayerState();

	virtual void BeginPlay() override;

	void GrantStartupAbilities();
	
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(VIsibleAnywhere, BlueprintReadOnly, Category = Abilities)
	TObjectPtr<class UFortAbilitySystemComponent> FortAbilitySystemComp;
	
	UPROPERTY()
	TObjectPtr<class UFortHealthAttributeSet> HealthSet;
};
