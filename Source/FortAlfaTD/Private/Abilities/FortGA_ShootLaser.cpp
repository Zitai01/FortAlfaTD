// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/FortGA_ShootLaser.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "FortEnemyBaseCharacter.h"
#include "FortTowerAttributeSet.h"
#include "FortTowerBase.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

UFortGA_ShootLaser::UFortGA_ShootLaser()
{
	AbilityTags.AddTag(
	FGameplayTag::RequestGameplayTag(FName("Abilities.Skill.LaserBeam"))
);
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	LaserFiringStateTag = FGameplayTag::RequestGameplayTag(TEXT("State.Firing.Laser"));
	bUseInstantHit = true;
}

void UFortGA_ShootLaser::PerformShoot(AFortTowerBase* Tower)
{
}

void UFortGA_ShootLaser::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AFortTowerBase* Tower = Cast<AFortTowerBase>(GetAvatarActorFromActorInfo());
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

	if (!Tower || !ASC || !Tower->GetCurrentTarget())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// Mark "laser firing" so TowerBase won't re-activate every tick
	ASC->AddLooseGameplayTag(LaserFiringStateTag);

	// Spawn beam VFX (skip on dedicated server)
	if (LaserBeamSystem)
	{
		UStaticMeshComponent* Turret = Tower->GetTurretMesh(); // use your actual getter
		const FName MuzzleSocket = TEXT("Barrel_End");

		if (Turret)
		{
			BeamComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
				LaserBeamSystem,
				Turret,
				Turret->DoesSocketExist(MuzzleSocket) ? MuzzleSocket : NAME_None,
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				EAttachLocation::SnapToTarget,
				false
			);
		}
	}

	// Start ticking (damage + beam follow)
	if (UWorld* World = Tower->GetWorld())
	{
		World->GetTimerManager().SetTimer(
			LaserTickHandle,
			this,
			&UFortGA_ShootLaser::TickLaser,
			DamageTickInterval,
			true
		);
	}
}

void UFortGA_ShootLaser::TickLaser()
{
	AFortTowerBase* Tower = Cast<AFortTowerBase>(GetAvatarActorFromActorInfo());
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	if (!Tower || !SourceASC)
	{
		StopBeam();
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	AFortEnemyBaseCharacter* Target = Tower->GetCurrentTarget();
	if (!Target || !Tower->IsEnemyValid(Target))
	{
		StopBeam();
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	// Update beam end every tick (no blinking)
	if (BeamComp)
	{
		BeamComp->SetVectorParameter(TEXT("User.Beam_End"), Target->GetActorLocation());
	}

	// Apply damage tick ONLY on authority
	if (!SourceASC->GetOwner() || !SourceASC->GetOwner()->HasAuthority())
	{
		return;
	}
	
	UAbilitySystemComponent* TargetASC = Target->GetAbilitySystemComponent();

	if (!SourceASC || !TargetASC)
	{
		return;
	}

	// Treat AttackDamage as DPS, apply per tick:
	float DPS = 2.f;
	if (const UFortTowerAttributeSet* Stats = Tower->GetTowerAttributes())
	{
		DPS = Stats->GetAttackDamage();
	}
	const float DamageThisTick = DPS * DamageTickInterval; // DamageTickInterval is your timer period

	FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
	Context.AddInstigator(Tower, nullptr);
	Context.AddSourceObject(Tower);

	FGameplayEffectSpecHandle SpecHandle =
		SourceASC->MakeOutgoingSpec(DamageGEClass, 1.f, Context);

	if (!SpecHandle.IsValid())
		return;

	SpecHandle.Data->SetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(FName("Data.Default.Damage")),
		DamageThisTick
	);

	SourceASC->ApplyGameplayEffectSpecToTarget(
		*SpecHandle.Data.Get(),
		TargetASC
	);
}

void UFortGA_ShootLaser::StopBeam()
{
	if (BeamComp)
	{
		BeamComp->Deactivate();
		BeamComp->DestroyComponent();
		BeamComp = nullptr;
	}
};

void UFortGA_ShootLaser::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	AFortTowerBase* Tower = Cast<AFortTowerBase>(GetAvatarActorFromActorInfo());
	if (Tower && Tower->GetWorld())
	{
		Tower->GetWorld()->GetTimerManager().ClearTimer(LaserTickHandle);
	}

	StopBeam();

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		ASC->RemoveLooseGameplayTag(LaserFiringStateTag);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
