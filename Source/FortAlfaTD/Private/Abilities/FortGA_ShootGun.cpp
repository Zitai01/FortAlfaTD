// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/FortGA_ShootGun.h"

UFortGA_ShootGun::UFortGA_ShootGun()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	FGameplayTag AbilityTag1 = FGameplayTag::RequestGameplayTag(FName("Abilities.Skill.ShootBullettBullet"));
	
	AbilityTags.AddTag(AbilityTag1);
	ActivationOwnedTags.AddTag(AbilityTag1);
	
}

void UFortGA_ShootGun::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo * ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData * TriggerEventData)
{
	
}