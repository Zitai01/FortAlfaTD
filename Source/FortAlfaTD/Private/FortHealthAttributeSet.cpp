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
