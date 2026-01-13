// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/FortGA_ShootLaser.h"

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
	
	bUseInstantHit = true;
}

void UFortGA_ShootLaser::PerformShoot(AFortTowerBase* Tower)
{
	if (!Tower || !Tower->CurrentTarget || !DamageGEClass)
		return;

	AFortEnemyBaseCharacter* Target = Tower->CurrentTarget;
	UAbilitySystemComponent* TargetASC = Target->GetAbilitySystemComponent();
	UAbilitySystemComponent* SourceASC = Tower->GetAbilitySystemComponent();

	if (!TargetASC || !SourceASC)
		return;

	// Damage from tower stats (same as base)
	float Damage = 2.f;
	if (const UFortTowerAttributeSet* Stats = Tower->GetTowerAttributes())
	{
		Damage = Stats->GetAttackDamage();
	}
	
	FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
	Context.AddInstigator(Tower, nullptr);
	Context.AddSourceObject(Tower);

	FGameplayEffectSpecHandle SpecHandle =
		SourceASC->MakeOutgoingSpec(DamageGEClass, 1.f, Context);

	if (!SpecHandle.IsValid())
		return;
	
	SpecHandle.Data->SetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(FName("Data.Default.Damage")),
		Damage
	);

	SourceASC->ApplyGameplayEffectSpecToTarget(
		*SpecHandle.Data.Get(),
		TargetASC
	);
	if (LaserBeamSystem && Tower && Tower->CurrentTarget)
	{
		const FVector Start = Tower->GetTurretMesh() && Tower->GetTurretMesh()->DoesSocketExist("Barrel_End")
			? Tower->GetTurretMesh()->GetSocketLocation("Barrel_End")
			: Tower->GetActorLocation();

		const FVector End = Tower->CurrentTarget->GetActorLocation();

		UNiagaraComponent* Comp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			Tower->GetWorld(),
			LaserBeamSystem,
			Start,
			FRotator::ZeroRotator
		);

		if (Comp)
		{
		//	Comp->SetVectorParameter(TEXT("User.BeamStart"), Start);
			Comp->SetVectorParameter(TEXT("User.Beam_End"), End);
		}
	}

}

void UFortGA_ShootLaser::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
};
