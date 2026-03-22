// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Pawn.h"

#include "Abilities/FortGA_ShootBase.h"
#include "Abilities/FortGA_ShootGun.h"

#include "FortTowerBase.generated.h"

class AFortPlayerState;
// Forward declarations (keep includes light)
class AFortEnemyBaseCharacter;
class AFortProjectileBase;
class UAbilitySystemComponent;
class UAudioComponent;
class UFortAbilityAsset;
class UFortAbilitySystemComponent;
class UFortHealthAttributeSet;
class UFortTowerAttributeSet;
class UNiagaraComponent;
class UNiagaraSystem;
class USceneComponent;
class USphereComponent;
class USoundBase;
class UStaticMeshComponent;
class UTowerData;
class UGameplayAbility;
class UFMODEvent;
class UFMODAudioComponent;

UCLASS()
class FORTALFATD_API AFortTowerBase : public APawn, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AFortTowerBase();
	
	// APawn
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Getters
	AFortEnemyBaseCharacter* GetCurrentTarget() const { return CurrentTarget; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Combat")
	TSubclassOf<AFortProjectileBase> GetProjectileClass() const { return ProjectileClass; }
	
	bool IsEnemyValid(APawn* Enemy);
	
	FORCEINLINE UFortTowerAttributeSet* GetTowerAttributes() const { return TowerAttributeSet; }
	FORCEINLINE UStaticMeshComponent* GetTurretMesh() const { return TurretMesh; }

	void NotifyShotFiredAudio();
	
	// Range callbacks
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

	// Public tweakables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	FGameplayTag AbilityTag;
	FVector TargetPredictedLocation;
protected:
	// AActor
	virtual void BeginPlay() override;

	// Core behavior


	AFortEnemyBaseCharacter* FindNearestEnemy();
	void TowerUpdate();
	void TryShoot(float DeltaTime);
	void RotateToFaceEnemy(float DeltaTime);
	void PredictTargetLocation(float ProjectileSpeed);

	// Audio
	void AudioUpdate();

	// Channeled / laser
	void EnsureChanneledAttackActive();
	void StopChanneledAttack();

	// LOS
	bool HasLineOfSightToTarget(AFortEnemyBaseCharacter* Target) const;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// -----------------------------
	// Combat / LOS
	// -----------------------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat|LOS")
	bool bRequireLineOfSight = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat|LOS")
	TEnumAsByte<ECollisionChannel> LineOfSightTraceChannel = ECC_Visibility;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat|LOS")
	float TargetAimZOffset = 40.f;

	// -----------------------------
	// Combat / GAS
	// -----------------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat|GAS")
	FGameplayTag PrimaryAttackTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat|GAS")
	bool bIsChanneledAttack = false; // laser-like: do not re-trigger while active

	FGameplayTag LaserFiringStateTag;

	UPROPERTY(EditDefaultsOnly, Category="Tower|Attack")
	FGameplayTag LaserAbilityTag;

	UPROPERTY(VIsibleAnywhere, BlueprintReadOnly, Category=Abilities)
	TObjectPtr<UFortAbilitySystemComponent> FortAbilitySystemComp;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UFortGA_ShootBase> ShootAbility;

	// -----------------------------
	// Tower identity / data
	// -----------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tower Info")
	FName TowerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tower Info")
	int32 TowerLevel = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tower", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UTowerData> TowerData;

	UPROPERTY(EditDefaultsOnly)
	UFortAbilityAsset* AbilitySet;

	// -----------------------------
	// Attributes / sets
	// -----------------------------
	UPROPERTY()
	TObjectPtr<UFortHealthAttributeSet> HealthSet;

	UPROPERTY()
	TObjectPtr<UFortTowerAttributeSet> TowerAttributeSet;

	// -----------------------------
	// Components (visual / collision)
	// -----------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Tower")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Tower")
	UStaticMeshComponent* MountMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Tower")
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Tower")
	USceneComponent* MuzzlePoint;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	USphereComponent* AttackRangeSphere;

	// -----------------------------
	// Projectile / VFX
	// -----------------------------
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<AFortProjectileBase> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* NiagaraSystem;

	UPROPERTY()
	TObjectPtr<UNiagaraComponent> ActiveHitScanComp;

	// -----------------------------
	// Audio
	// -----------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Audio")
	UAudioComponent* RotatingAudioComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Audio")
	USoundBase* BaseRotationMetaSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Audio")
	float RotationStartThresholdDegPerSec = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Audio")
	float RotationStopThresholdDegPerSec = .5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Audio")
	float RotationStopFadeSec = .05f;

	bool bRotationAudioPlaying = false;
	float MountRotationYawDeg = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Audio|FMOD")
	TObjectPtr<UFMODEvent> FireOneShotEvent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Audio|FMOD")
	TObjectPtr<UFMODEvent> FireLoopEvent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Audio|FMOD")
	TObjectPtr<UFMODEvent> FireLoopStopEvent = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Audio|FMOD")
	bool bUseLoopedGunFireAudio = false;
	
	UPROPERTY(Transient)
	TObjectPtr<UFMODAudioComponent> FireLoopAudioComp = nullptr;

	UPROPERTY(Transient)
	bool bAudioFiringActive = false;

	UPROPERTY(Transient)
	float LastShotAudioTime = -1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Audio|FMOD")
	float GunLoopHoldSeconds = 0.12f;
	
	// -----------------------------
	// Runtime state
	// -----------------------------
	UPROPERTY()
	TArray<AFortEnemyBaseCharacter*> EnemiesInRange;

	TArray<TObjectPtr<AFortEnemyBaseCharacter>> EnemiesWithInRange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Tower")
	AFortEnemyBaseCharacter* CurrentTarget = nullptr;
	float TimeSinceLastShot = 0.f;

	FTimerHandle TowerLogicTimerHandle;
	FTimerHandle AudioLogicTimerHandle;

private:
	FActiveGameplayEffectHandle TechGEHandle;
	TWeakObjectPtr<AFortPlayerState> CachedOwnerPS;

	void BindTechListener();
	void UnbindTechListener();
	AFortPlayerState* ResolveFortPlayerState() const;
	void ReapplyTech(); // remove old handle, apply new, refresh range sphere
	void HandleTechChanged();

	//Audio Helpers

	void StartFireLoopAudio();
	void StopFireLoopAudio(bool bPlayStopEvent = true);
	bool ShouldHaveLoopedFireAudio() const;
};