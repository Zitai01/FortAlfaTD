// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/FortDefenseAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemComponent.h"
#include "FortHealthAttributeSet.h"

UFortDefenseAttributeSet::UFortDefenseAttributeSet()
{
	InitArmor(0.0f);
	InitShield(0.0f);
	InitMaxShield(0.0f);
	InitIncomingShieldDamage(0.0f);
}


void UFortDefenseAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetArmorAttribute())
	{
		NewValue = FMath::Max(0.0f, NewValue);
	}
	else if (Attribute == GetShieldAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxShield());
	}
	else if (Attribute == GetMaxShieldAttribute())
	{
		NewValue = FMath::Max(0.0f, NewValue);
	}
	else if (Attribute == GetIncomingShieldDamageAttribute())
	{
		NewValue = FMath::Max(0.0f, NewValue);
	}
}

void UFortDefenseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetIncomingShieldDamageAttribute())
	{
		const float ShieldDamage = GetIncomingShieldDamage();
		SetIncomingShieldDamage(0.0f);

		if (ShieldDamage > 0.0f)
		{
			const float NewShield = FMath::Clamp(GetShield() - ShieldDamage, 0.0f, GetMaxShield());
			SetShield(NewShield);
		}
	}
	else if (Data.EvaluatedData.Attribute == GetShieldAttribute())
	{
		SetShield(FMath::Clamp(GetShield(), 0.0f, GetMaxShield()));
	}
	else if (Data.EvaluatedData.Attribute == GetMaxShieldAttribute())
	{
		SetMaxShield(FMath::Max(0.0f, GetMaxShield()));
		SetShield(FMath::Clamp(GetShield(), 0.0f, GetMaxShield()));
	}
	else if (Data.EvaluatedData.Attribute == GetArmorAttribute())
	{
		SetArmor(FMath::Max(0.0f, GetArmor()));
	}
}
