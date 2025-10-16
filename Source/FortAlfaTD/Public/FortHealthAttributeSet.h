// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "FortAbilitySystemComponent.h"
#include "FortHealthAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS()
class FORTALFATD_API UFortHealthAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

	public:
	UFortHealthAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Replicated)
	FGameplayAttributeData Health;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Replicated)
	FGameplayAttributeData MaxHealth;

	ATTRIBUTE_ACCESSORS(UFortHealthAttributeSet,Health);
	ATTRIBUTE_ACCESSORS(UFortHealthAttributeSet,MaxHealth);
	
};
