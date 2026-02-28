// Fill out your copyright notice in the Description page of Project Settings.


#include "FortPlayerState.h"
#include "AbilitySystemComponent.h"
#include "FortAbilitySystemComponent.h"
#include "FortHealthAttributeSet.h"
#include "TowerData.h"
#include "Data/FortTechNodeData.h"
#include "Data/FortTechTreeData.h"

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

void AFortPlayerState::InitTechFromTree()
{
	UFortTechTreeData* Tree = ActiveTechTree.Get();
	if (!Tree) return;

	UnlockedNodeIDs.Reset();
	for (const FName& Id : Tree->StartingUnlockedNodeIDs)
	{
		UnlockedNodeIDs.Add(Id);
	}

	OnTechChanged.Broadcast();
}

const UFortTechNodeData* AFortPlayerState::FindTechNodeByID(const UFortTechTreeData* Tree, FName NodeID) const
{
	if (!Tree) return nullptr;

	for (const FFortTechNodeUIEntry& Entry : Tree->Nodes)
	{
		if (Entry.Node && Entry.Node->NodeID == NodeID)
		{
			return Entry.Node;
		}
	}
	return nullptr;
}

bool AFortPlayerState::CanUnlockTechNode(const UFortTechNodeData* Node) const
{
	if (!Node) return false;
	if (UnlockedNodeIDs.Contains(Node->NodeID)) return false;

	for (const FName& Req : Node->PrerequisiteNodeIDs)
	{
		if (!UnlockedNodeIDs.Contains(Req)) return false;
	}

	// NEW: check multi-currency
	for (const FFortTechCost& Cost : Node->Costs)
	{
		if (!Cost.CurrencyTag.IsValid()) return false;
		if (GetCurrency(Cost.CurrencyTag) < Cost.Amount) return false;
	}

	return true;
}

bool AFortPlayerState::TryUnlockTechNode(FName NodeID)
{
	UFortTechTreeData* Tree = ActiveTechTree.Get();
	const UFortTechNodeData* Node = FindTechNodeByID(Tree, NodeID);
	if (!CanUnlockTechNode(Node)) return false;

	// Spend
	for (const FFortTechCost& Cost : Node->Costs)
	{
		ResearchCurrency.FindOrAdd(Cost.CurrencyTag) -= Cost.Amount;
	}

	UnlockedNodeIDs.Add(Node->NodeID);
	OnTechChanged.Broadcast();
	return true;
}

void AFortPlayerState::BuildTechMagnitudesForTower(const UTowerData* TowerData, TMap<FGameplayTag, float>& OutTagToMagnitude) const
{
	OutTagToMagnitude.Reset();

	UFortTechTreeData* Tree = ActiveTechTree.Get();
	if (!Tree) return;

	for (const FFortTechNodeUIEntry& Entry : Tree->Nodes)
	{
		const UFortTechNodeData* Node = Entry.Node;
		if (!Node) continue;
		if (!UnlockedNodeIDs.Contains(Node->NodeID)) continue;

		// Global applies to all
		bool bApplies = Node->TargetTowerData.IsNull();

		// Tower-specific applies only if TowerData matches
		if (!bApplies && TowerData)
		{
			UTowerData* Target = Node->TargetTowerData.Get();
			bApplies = (Target == TowerData);
		}

		if (!bApplies) continue;

		for (const FFortTechSetByCallerMod& Mod : Node->SetByCallerMods)
		{
			if (!Mod.DataTag.IsValid()) continue;
			OutTagToMagnitude.FindOrAdd(Mod.DataTag) += Mod.Magnitude;
		}
	}
}

FActiveGameplayEffectHandle AFortPlayerState::ApplyTechToTowerASC(UAbilitySystemComponent* ASC, const UTowerData* TowerData) const
{
	if (!ASC || !TechModifierGE) return FActiveGameplayEffectHandle();

	TMap<FGameplayTag, float> TagToMag;
	BuildTechMagnitudesForTower(TowerData, TagToMag);

	FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(TechModifierGE, 1.f, Ctx);
	if (!SpecHandle.IsValid()) return FActiveGameplayEffectHandle();

	FGameplayEffectSpec* Spec = SpecHandle.Data.Get();
	for (const auto& It : TagToMag)
	{
		Spec->SetSetByCallerMagnitude(It.Key, It.Value);
	}

	return ASC->ApplyGameplayEffectSpecToSelf(*Spec);
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

int32 AFortPlayerState::GetCurrency(FGameplayTag CurrencyTag) const
{
	if (!CurrencyTag.IsValid()) return 0;
	if (const int32* Found = ResearchCurrency.Find(CurrencyTag))
	{
		return *Found;
	}
	return 0;
}

void AFortPlayerState::AddCurrency(FGameplayTag CurrencyTag, int32 Delta)
{
	if (!CurrencyTag.IsValid() || Delta == 0) return;
	ResearchCurrency.FindOrAdd(CurrencyTag) += Delta;
}