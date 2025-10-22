// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayEffectExtension.h"
#include "FortAbilitySystemComponent.h"
#include "FortHealthAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttributeChangedEvent, UAttributeSet*, AttributeSet, float, OldValue, float, NewValue);

UCLASS()
class FORTALFATD_API UFortHealthAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

	public:
	UFortHealthAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Replicated, meta = (HideFromModifiers))
	FGameplayAttributeData Health;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Replicated)
	FGameplayAttributeData MaxHealth;


	UPROPERTY(BlueprintAssignable)
	FAttributeChangedEvent OnHealthChanged;

	UPROPERTY(VisibleAnywhere)
	FGameplayAttributeData Damage;
	
	ATTRIBUTE_ACCESSORS(UFortHealthAttributeSet, Damage);
	ATTRIBUTE_ACCESSORS(UFortHealthAttributeSet,Health);
	ATTRIBUTE_ACCESSORS(UFortHealthAttributeSet,MaxHealth);
	
};
