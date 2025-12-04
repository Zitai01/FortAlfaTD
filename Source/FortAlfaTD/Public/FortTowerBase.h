// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Abilities/FortGA_ShootBase.h"
#include "Abilities/FortGA_ShootGun.h"
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
	
	AFortEnemyBaseCharacter* FindNearestEnemy();
	
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
	TArray<AFortEnemyBaseCharacter*> EnemiesInRange;

	TArray<TObjectPtr<AFortEnemyBaseCharacter>> EnemiesWithInRange;
	
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
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UFortGA_ShootGun> ShootAbility;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<AFortProjectileBase> ProjectileClass;



	float TimeSinceLastShot = 0.f;
	FTimerHandle TowerLogicTimerHandle;
public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Combat")
	TSubclassOf< AFortProjectileBase> GetProjectileClass() const { return ProjectileClass; }

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	FORCEINLINE UFortTowerAttributeSet* GetTowerAttributes() const { return TowerAttributeSet; }

public:
	FORCEINLINE UStaticMeshComponent* GetTurretMesh() const { return TurretMesh; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

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
};
