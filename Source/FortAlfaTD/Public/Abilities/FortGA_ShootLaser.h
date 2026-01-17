// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/FortGA_ShootBase.h"
#include "FortGA_ShootLaser.generated.h"

class UNiagaraComponent;
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
//	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:

	virtual void ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled) override;

	void TickLaser();          // updates beam end + applies damage tick
	void StopBeam();
	
	UPROPERTY(EditDefaultsOnly, Category="Laser|VFX")
	UNiagaraSystem* LaserBeamSystem = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Laser")
	float DamageTickInterval = 0.03f;
	
	UPROPERTY(EditDefaultsOnly, Category="Laser")
	TSubclassOf<UGameplayEffect> DamageGEClass;

	UPROPERTY()
	TObjectPtr<UNiagaraComponent> BeamComp;

	
	FTimerHandle LaserTickHandle;

	FGameplayTag LaserFiringStateTag;
};
