// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "FortProjectileBase.generated.h"

class UNiagaraComponent;

UCLASS()
class FORTALFATD_API AFortProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFortProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	float Damage = 10.f;

public:	

	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UNiagaraComponent> NiagaraComp;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile | Components")
	TObjectPtr<USphereComponent> CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	TSubclassOf<UGameplayEffect> DamageGEClass;

	void FireInDirection(const FVector& ShootDirection);
	
	UFUNCTION()
	void HandleImpact(    UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult);
	virtual void Tick( float DeltaTime ) override;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	USphereComponent* AttackRange;
	
	void SetTarget(AActor* InTarget) { Target = InTarget; }
	void SetDamage(float InDamage) { Damage = InDamage; }
    void ApplyDamageTo(AActor* OtherActor);
	AActor* Target;
};
