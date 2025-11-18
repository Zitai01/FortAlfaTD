// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
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

public:	

	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UNiagaraComponent> NiagaraComp;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UStaticMesh> StaticMeshComp;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class USphereComponent* AttackRange;
};
