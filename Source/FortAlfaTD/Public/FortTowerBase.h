// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Abilities/FortGA_ShootBase.h"
#include "Abilities/FortGA_ShootGun.h"
#include "GameFramework/Pawn.h"
#include "FortTowerBase.generated.h"

class UNiagaraSystem;
class UFortAbilityAsset;
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
	
	AFortEnemyBaseCharacter* FindNearestEnemy();
	
	void RotateToFaceEnemy(float DeltaTime);
	
	void TryShoot(float DeltaTime);

	void AudioUpdate();
	
	void TowerUpdate();
	
	void PredictTargetLocation(float ProjectileSpeed) ;
	
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
	TArray<AFortEnemyBaseCharacter*> EnemiesInRange;

	TArray<TObjectPtr<AFortEnemyBaseCharacter>> EnemiesWithInRange;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Tower")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Tower")
	UStaticMeshComponent* MountMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Tower")
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Audio")
	UAudioComponent* RotatingAudioComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Audio")
	USoundBase* BaseRotationMetaSound;
	
	bool bRotationAudioPlaying  = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Audio")
	float RotationStartThresholdDegPerSec  = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Audio")
	float RotationStopThresholdDegPerSec  = .5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Audio")
	float RotationStopFadeSec = .05f;

	float MountRotationYawDeg = 0.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Tower")
	USceneComponent* MuzzlePoint;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	USphereComponent* AttackRangeSphere;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UFortGA_ShootBase> ShootAbility;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<AFortProjectileBase> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	UFortAbilityAsset* AbilitySet; 

	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* NiagaraSystem;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat|GAS")
	FGameplayTag PrimaryAttackTag;

	float TimeSinceLastShot = 0.f;
	
	FTimerHandle TowerLogicTimerHandle;
	
	FTimerHandle AudioLogicTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat|GAS")
	bool bIsChanneledAttack = false; // laser-like: do not re-trigger while active


public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Combat")
	TSubclassOf< AFortProjectileBase> GetProjectileClass() const { return ProjectileClass; }

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	FORCEINLINE UFortTowerAttributeSet* GetTowerAttributes() const { return TowerAttributeSet; }
	
	FORCEINLINE UStaticMeshComponent* GetTurretMesh() const { return TurretMesh; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	FGameplayTag AbilityTag;
	
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Tower")
	AFortEnemyBaseCharacter* CurrentTarget = nullptr;

	FVector TargetPredictedLocation;
};
