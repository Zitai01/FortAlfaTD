// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FortGA_ShootBase.generated.h"

class AFortProjectileBase;
/**
 * 
 */
UCLASS()
class FORTALFATD_API UFortGA_ShootBase : public UGameplayAbility
{
	GENERATED_BODY()


public:
	UFortGA_ShootBase();
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UAnimMontage* FireHipMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UAnimMontage* FireIronsightsMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<AFortProjectileBase> ProjectileClass;

	void PerformShoot(class AFortTowerBase* Tower);
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseInstantHit = false;
};
