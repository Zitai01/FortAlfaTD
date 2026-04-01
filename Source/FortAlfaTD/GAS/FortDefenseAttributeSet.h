// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "FortDefenseAttributeSet.generated.h"
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class FORTALFATD_API UFortDefenseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UFortDefenseAttributeSet();
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	/** Flat armor used by damage execution */
	UPROPERTY(BlueprintReadOnly, Category = "Defense")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UFortDefenseAttributeSet, Armor)

	/** Current shield */
	UPROPERTY(BlueprintReadOnly, Category = "Defense")
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(UFortDefenseAttributeSet, Shield)

	/** Max shield */
	UPROPERTY(BlueprintReadOnly, Category = "Defense")
	FGameplayAttributeData MaxShield;
	ATTRIBUTE_ACCESSORS(UFortDefenseAttributeSet, MaxShield)

	/**
	 * Meta attribute written by damage execution.
	 * Execution decides how much shield damage should be applied,
	 * this attribute set only consumes it.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Defense")
	FGameplayAttributeData IncomingShieldDamage;
	ATTRIBUTE_ACCESSORS(UFortDefenseAttributeSet, IncomingShieldDamage)

};