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
	SetNetUpdateFrequency(100);
}

UAbilitySystemComponent* AFortPlayerState::GetAbilitySystemComponent() const
{
	return  FortAbilitySystemComp;
}

void AFortPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		GrantStartupAbilities();
	}
}

void AFortPlayerState::GrantStartupAbilities()
{
	/*
	for (TSubclassOf<UGameplayAbility> Ability : StartupAbilities)
	{
		FortAbilitySystemComp->GiveAbility(
			FGameplayAbilitySpec(Ability, 1)
		);
	}
	*/
}
