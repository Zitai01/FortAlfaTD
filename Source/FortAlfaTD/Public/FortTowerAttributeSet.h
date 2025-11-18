// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "FortAbilitySystemComponent.h"
#include "FortTowerAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class FORTALFATD_API UFortTowerAttributeSet : public UAttributeSet
{
	GENERATED_BODY()


public:
	UFortTowerAttributeSet();

	UPROPERTY(BlueprintReadOnly, Category="Tower")
	FGameplayAttributeData AttackSpeed;


	UPROPERTY(BlueprintReadOnly, Category="Tower")
	FGameplayAttributeData AttackDamage;

	UPROPERTY(BlueprintReadOnly, Category="Tower")
	FGameplayAttributeData AttackRange;

	ATTRIBUTE_ACCESSORS(UFortTowerAttributeSet, AttackSpeed)
	ATTRIBUTE_ACCESSORS(UFortTowerAttributeSet, AttackDamage)
	ATTRIBUTE_ACCESSORS(UFortTowerAttributeSet, AttackRange)
};
