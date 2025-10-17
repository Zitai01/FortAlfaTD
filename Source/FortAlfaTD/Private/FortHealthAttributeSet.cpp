// Fill out your copyright notice in the Description page of Project Settings.


#include "FortHealthAttributeSet.h"
#include "FortAlfaTD.h"

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
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		// When max health changes, broadcast OnHealthChanged so that health bars will update
		const float CurrentHealth = GetHealth();
		OnHealthChanged.Broadcast(this, CurrentHealth, CurrentHealth);
	}
}