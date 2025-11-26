// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FortEnemySpawnPoint.generated.h"

UCLASS()
class FORTALFATD_API AFortEnemySpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFortEnemySpawnPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawn")
	int32 LaneIndex = 0;  // Which lane this spawn point belongs to

};
