// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "FortEnemyBaseCharacter.generated.h"

class UGameplayEffect;
class USphereComponent;
class UFortEnemyDataAsset;

UCLASS(abstract)
class FORTALFATD_API AFortEnemyBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties


protected:
	UPROPERTY(VIsibleAnywhere, BlueprintReadOnly, Category = Abilities)
	TObjectPtr<class UFortAbilitySystemComponent> FortAbilitySystemComp;

	UPROPERTY()
	TObjectPtr<class UFortHealthAttributeSet> HealthSet;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// ===== Melee =====
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat")
	TObjectPtr<USphereComponent> MeleeRangeSphere;

	UPROPERTY(EditDefaultsOnly, Category="Combat|Melee")
	float MeleeRange = 180.f;

	UPROPERTY(EditDefaultsOnly, Category="Combat|Melee")
	float MeleeInterval = 0.8f;

	UPROPERTY(EditDefaultsOnly, Category="Combat|Melee")
	float MeleeDamage = 10.f;

	UPROPERTY(EditDefaultsOnly, Category="Combat|Melee")
	TSubclassOf<UGameplayEffect> DamageGEClass;

	FTimerHandle MeleeTimerHandle;
	TWeakObjectPtr<AActor> CurrentMeleeTarget;

	UFUNCTION()
	void OnMeleeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
							bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnMeleeEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void StartMelee();
	void StopMelee();
	void TryMeleeAttack();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	AFortEnemyBaseCharacter();
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void InitEnemyFromData(UFortEnemyDataAsset* DataAsset);
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
};
