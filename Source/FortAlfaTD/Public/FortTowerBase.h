// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Pawn.h"
#include "FortTowerBase.generated.h"

class USphereComponent;
class UAbilitySystemComponent;
class UGameplayAbility;


UCLASS()
class FORTALFATD_API AFortTowerBase : public APawn,  public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFortTowerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VIsibleAnywhere, BlueprintReadOnly, Category = Abilities)
	TObjectPtr<class UFortAbilitySystemComponent> FortAbilitySystemComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tower Info")
	FName TowerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tower Info")
	int32 TowerLevel = 1;
	
	UPROPERTY()
	TObjectPtr<class UFortHealthAttributeSet> HealthSet;

	UPROPERTY()
	TObjectPtr<class UFortTowerAttributeSet> TowerAttributeSet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class USphereComponent* AttackRange;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

};
