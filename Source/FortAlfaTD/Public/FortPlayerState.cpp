// Fill out your copyright notice in the Description page of Project Settings.


#include "FortPlayerState.h"
#include "AbilitySystemComponent.h"
#include "FortAbilitySystemComponent.h"
#include "FortHealthAttributeSet.h"

AFortPlayerState::AFortPlayerState()
{
	FortAbilitySystemComp = CreateDefaultSubobject<UFortAbilitySystemComponent>(TEXT("ASC"));
	HealthSet = CreateDefaultSubobject<UFortHealthAttributeSet>(TEXT("HealthSet"));
	FortAbilitySystemComp->SetIsReplicated(true);
}

UAbilitySystemComponent* AFortPlayerState::GetAbilitySystemComponent() const
{
	return  FortAbilitySystemComp;
}