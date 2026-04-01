// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/FortDamageExecutions.h"

#include "FortDefenseAttributeSet.h"


namespace FortDamageStatics
{
	struct FDamageCaptureDefs
	{
		DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
		DECLARE_ATTRIBUTE_CAPTUREDEF(Shield);

		FDamageCaptureDefs()
		{
			DEFINE_ATTRIBUTE_CAPTUREDEF(UFortDefenseAttributeSet, Armor, Target, false);
			DEFINE_ATTRIBUTE_CAPTUREDEF(UFortDefenseAttributeSet, Shield, Target, false);
		}
	};

	static const FDamageCaptureDefs& Get()
	{
		static FDamageCaptureDefs Statics;
		return Statics;
	}
}

void UFortDamageExecutions::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
	
}
