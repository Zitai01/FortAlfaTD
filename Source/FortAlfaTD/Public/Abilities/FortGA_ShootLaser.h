// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/FortGA_ShootBase.h"
#include "FortGA_ShootLaser.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class FORTALFATD_API UFortGA_ShootLaser : public UFortGA_ShootBase
{
	GENERATED_BODY()

public:
	UFortGA_ShootLaser();


	virtual void PerformShoot(AFortTowerBase* Tower) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:

	UPROPERTY(EditDefaultsOnly, Category="Laser|VFX")
	UNiagaraSystem* LaserBeamSystem = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Laser")
	TSubclassOf<UGameplayEffect> DamageGEClass;
};
