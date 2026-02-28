// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameFramework/PlayerState.h"
#include "NativeGameplayTags.h"
#include "FortPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class FORTALFATD_API AFortPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AFortPlayerState();

	virtual void BeginPlay() override;

	void GrantStartupAbilities();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// TechTree data + state
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tech")
	TSoftObjectPtr<class UFortTechTreeData> ActiveTechTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tech")
	TSubclassOf<class UGameplayEffect> TechModifierGE;

	UPROPERTY(BlueprintReadOnly, Category="Tech")
	TSet<FName> UnlockedNodeIDs;

	UPROPERTY(BlueprintReadOnly, Category="Tech")
	TMap<FGameplayTag, int32> ResearchCurrency;

	// Optional: if you want UI to react
	DECLARE_MULTICAST_DELEGATE(FOnTechChanged);
	FOnTechChanged OnTechChanged;

	UFUNCTION(BlueprintCallable, Category="Tech")
	void InitTechFromTree();

	UFUNCTION(BlueprintCallable, Category="Tech")
	bool TryUnlockTechNode(FName NodeID);

	UFUNCTION(BlueprintCallable, Category="Tech")
	int32 GetCurrency(FGameplayTag CurrencyTag) const;

	UFUNCTION(BlueprintCallable, Category="Tech")
	void AddCurrency(FGameplayTag CurrencyTag, int32 Delta);
	
	void BuildTechMagnitudesForTower(const class UTowerData* TowerData, TMap<FGameplayTag, float>& OutTagToMagnitude) const;

	// Applies tech and returns handle so the tower can remove/reapply later
	FActiveGameplayEffectHandle ApplyTechToTowerASC(class UAbilitySystemComponent* ASC, const UTowerData* TowerData) const;

private:
	const class UFortTechNodeData* FindTechNodeByID(const class UFortTechTreeData* Tree, FName NodeID) const;
	bool CanUnlockTechNode(const class UFortTechNodeData* Node) const;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities)
	TObjectPtr<class UFortAbilitySystemComponent> FortAbilitySystemComp;
	
	UPROPERTY()
	TObjectPtr<class UFortHealthAttributeSet> HealthSet;
};
