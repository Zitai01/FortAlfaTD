// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/FortGA_ShootBase.h"
#include "FortEnemyBaseCharacter.h"
#include "FortHealthAttributeSet.h"
#include "FortTowerBase.h"
#include "Projectiles/FortProjectileBase.h"
#include "FortTowerAttributeSet.h"

UFortGA_ShootBase::UFortGA_ShootBase()
{
	FireCueTag = FGameplayTag::RequestGameplayTag(FName("GameplayCue.Tower.MachineGunFire"));
}

void UFortGA_ShootBase::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	AFortTowerBase* Tower = Cast<AFortTowerBase>(GetAvatarActorFromActorInfo());
	if (!Tower || !Tower->CurrentTarget)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// Commit cost/cooldown
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// Shoot logic
	PerformShoot(Tower);

	// End ability immediately
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UFortGA_ShootBase::PerformShoot(AFortTowerBase* Tower)
{
	AFortEnemyBaseCharacter* Target = Tower->CurrentTarget;
	if (!Target) return;

	UFortTowerAttributeSet* Stats = Tower->GetTowerAttributes();
	float Damage = Stats ? Stats->GetAttackDamage() : 10.f;
	
	FVector MuzzleLoc;
	FRotator MuzzleRot;
	FVector Direction;
	if (Tower->GetTurretMesh() && Tower->GetTurretMesh()->DoesSocketExist("Barrel_End"))
	{
		MuzzleLoc = Tower->GetTurretMesh()->GetSocketLocation("Barrel_End");
		MuzzleRot = Tower->GetTurretMesh()->GetSocketRotation("Barrel_End");
		Direction = (Tower->TargetPredictedLocation  - MuzzleLoc).GetSafeNormal();
	}
	else
	{
		// fallback
		MuzzleLoc = Tower->GetActorLocation();
		MuzzleRot = (Tower->TargetPredictedLocation - MuzzleLoc).Rotation();
		Direction = (Tower->TargetPredictedLocation - MuzzleLoc).GetSafeNormal();
	}

	// If instant hit mode
	if (bUseInstantHit)
	{
		// Apply damage instantly
		if (UAbilitySystemComponent* EnemyASC = Target->GetAbilitySystemComponent())
		{
			// You can apply a real GameplayEffect instead — I can write that too
			EnemyASC->ApplyModToAttributeUnsafe(
				FGameplayAttribute(UFortHealthAttributeSet::GetHealthAttribute()),
				EGameplayModOp::Additive,
				-Damage
			);
		}
		return;
	}

	// Spawn projectile version
	ProjectileClass = Tower->GetProjectileClass();
	if (ProjectileClass)
	{
		FActorSpawnParameters Params;
		Params.Owner = Tower;
		FVector SpawnAlignLocation = FVector(0,0,0);
		AFortProjectileBase* Projectile = Tower->GetWorld()->SpawnActor<AFortProjectileBase>(
			ProjectileClass,
			MuzzleLoc + SpawnAlignLocation ,
			MuzzleRot,
			Params
		);

		if (Projectile)
		{
			Projectile->SetTarget(Target);
			Projectile->SetDamage(Damage);
			Projectile->FireInDirection(Direction);
			Projectile->SetTarget(Tower->CurrentTarget); 
		}
	}
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	check(ASC);
	check(FireCueTag.IsValid());

	FGameplayCueParameters CueParams;
	CueParams.Location = MuzzleLoc;  
	CueParams.Instigator = GetAvatarActorFromActorInfo(); 

	ASC->ExecuteGameplayCue(FireCueTag, CueParams);


}
