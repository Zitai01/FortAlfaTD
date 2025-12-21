// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "FortEnemyAttributeSet.generated.h"
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS()
class FORTALFATD_API UFortEnemyAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category="Enemy")
	FGameplayAttributeData AttackSpeed;

	
	UPROPERTY(BlueprintReadOnly, Category="Enemy")
	FGameplayAttributeData AttackDamage;

	UPROPERTY(BlueprintReadOnly, Category="Enemy")
	FGameplayAttributeData AttackRange;

	UPROPERTY(BlueprintReadOnly, Category="Enemy")
	FGameplayAttributeData ProjectileSpeed;

	UFortEnemyAttributeSet();
	ATTRIBUTE_ACCESSORS(UFortEnemyAttributeSet, ProjectileSpeed)
	ATTRIBUTE_ACCESSORS(UFortEnemyAttributeSet, AttackSpeed)
	ATTRIBUTE_ACCESSORS(UFortEnemyAttributeSet, AttackDamage)
	ATTRIBUTE_ACCESSORS(UFortEnemyAttributeSet, AttackRange)
};
