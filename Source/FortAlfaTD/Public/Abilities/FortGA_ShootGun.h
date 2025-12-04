// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/FortGA_ShootBase.h"
#include "FortGA_ShootGun.generated.h"

/**
 * 
 */
UCLASS()
class FORTALFATD_API UFortGA_ShootGun : public UFortGA_ShootBase
{
	GENERATED_BODY()

public:
	UFortGA_ShootGun();

	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void PerformShoot( AFortTowerBase* Tower);
protected:

};
