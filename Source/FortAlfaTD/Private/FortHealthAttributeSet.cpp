// Fill out your copyright notice in the Description page of Project Settings.


#include "FortHealthAttributeSet.h"
#include "FortAlfaTD.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UFortHealthAttributeSet::UFortHealthAttributeSet()
{
	InitHealth(100.0f);
	InitMaxHealth(100.0f);
}

void UFortHealthAttributeSet::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UFortHealthAttributeSet, MaxHealth);
	DOREPLIFETIME(UFortHealthAttributeSet, Health);
}

void UFortHealthAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	UE_LOG(LogTemp, Warning, TEXT("PreChange: Attribute '%s'"), *Attribute.AttributeName);
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	Super::PreAttributeChange(Attribute, NewValue);
}
void UFortHealthAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	UE_LOG(LogTemp, Warning, TEXT("PostChange: Attribute '%s' changed %.2f -> %.2f"), *Attribute.AttributeName, OldValue, NewValue);
 
	if (Attribute == GetHealthAttribute())
	{
		OnHealthChanged.Broadcast(this, OldValue, NewValue);
		if (NewValue <= 0.0f && OldValue > 0.0f)
		{
			if (UAbilitySystemComponent* OwningAbilitySystemComponent = GetValid(GetOwningAbilitySystemComponent()) )
			{

				
				if (AActor* OwnerActor = GetOwningActor())
				{
					const FGameplayTag DeathTag = FGameplayTag::RequestGameplayTag(FName("State.Death.Start"), /*ErrorIfNotFound=*/true);
					OwningAbilitySystemComponent->HandleGameplayEvent(DeathTag,nullptr);
				}
			}			
		}
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		// When max health changes, broadcast OnHealthChanged so that health bars will update
		const float CurrentHealth = GetHealth();
		OnHealthChanged.Broadcast(this, CurrentHealth, CurrentHealth);
	}

}

void UFortHealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	UE_LOG(LogTemp, Warning, TEXT("PostApply: Gameplay Effect '%s' effect"), *Data.EffectSpec.Def->GetClass()->GetName());

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Convert into -Health and then clamp
		const float DamageValue = GetDamage();
		const float OldHealthValue = GetHealth();
		const float MaxHealthValue = GetMaxHealth();
		const float NewHealthValue = FMath::Clamp(OldHealthValue - DamageValue, 0.0f, MaxHealthValue);


		if (OldHealthValue != NewHealthValue)
		{
			// Set the new health after clamping to min-max
			SetHealth(NewHealthValue);
			// Calculate 'actual' damage applied that respects min and max health
			const float DamageNumber = OldHealthValue - NewHealthValue;
			if (UAbilitySystemComponent* OwningAbilitySystemComponent = GetValid(GetOwningAbilitySystemComponent()))
			{
				// Broadcast a 'damage number' gameplay cue on the owning actor. Triggered on server, executes on all clients.
				const FGameplayTag DamageCueTag = FGameplayTag::RequestGameplayTag(FName("GameplayCue.DamageNumber"), /*ErrorIfNotFound=*/true);
				FGameplayCueParameters DamageCueParams;
				DamageCueParams.NormalizedMagnitude = 1.f;
				DamageCueParams.RawMagnitude = DamageNumber;
				OwningAbilitySystemComponent->ExecuteGameplayCue(DamageCueTag, DamageCueParams);
			}

		}
		


		
		// Clear the meta attribute that temporarily held damage
		SetDamage(0.0f);
	}
}
