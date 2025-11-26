// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Abilities/FortGA_ShootBase.h"
#include "GameFramework/Pawn.h"
#include "FortTowerBase.generated.h"

class AFortEnemyBaseCharacter;
class USphereComponent;
class UAbilitySystemComponent;
class UGameplayAbility;


UCLASS()
class FORTALFATD_API AFortTowerBase : public APawn,  public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	AFortTowerBase();
	

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool IsEnemyValid(APawn* Enemy);
	
	APawn* FindNearestEnemy();
	
	void RotateToFaceEnemy(float DeltaTime);
	
	void TryShoot(float DeltaTime);
	
	void TowerUpdate();
	
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

	UPROPERTY()
	TArray<APawn*> EnemiesInRange;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	USphereComponent* AttackRangeSphere;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tower")
	float AttackRange = 1500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tower")
	float AttackSpeed = 1.0f; // seconds between shots

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Tower")
	AFortEnemyBaseCharacter* CurrentTarget = nullptr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UFortGA_ShootBase> ShootAbility;
	
	float TimeSinceLastShot = 0.f;
	FTimerHandle TowerLogicTimerHandle;
public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION()
	void OnEnemyEnterRange(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnEnemyExitRange(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
	
};
