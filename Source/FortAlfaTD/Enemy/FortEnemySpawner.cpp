// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/FortEnemySpawner.h"

#include "FortEnemySpawnPoint.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFortEnemySpawner::AFortEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AFortEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	FindSpawnPointsInLevel();
}

void AFortEnemySpawner::FindSpawnPointsInLevel()
{

}

void AFortEnemySpawner::StartWave(int32 WaveIndex)
{

}

void AFortEnemySpawner::StartNextWave()
{

}

void AFortEnemySpawner::SetupWaveState()
{

}

void AFortEnemySpawner::SpawnNextGroup()
{

}

void AFortEnemySpawner::SpawnEnemy()
{

}

AFortEnemySpawnPoint* AFortEnemySpawner::GetSpawnPointForLane(int32 LaneIndex) const
{
	for (AFortEnemySpawnPoint* SP : SpawnPoints)
	{
		if (SP && SP->LaneIndex == LaneIndex)
		{
			return SP;
		}
	}

	// If no matching lane, fall back to first spawn point
	return SpawnPoints.Num() > 0 ? SpawnPoints[0] : nullptr;
}

void AFortEnemySpawner::RegisterEnemyDeath(AFortEnemyBaseCharacter* DeadEnemy)
{
	EnemiesAlive = FMath::Max(0, EnemiesAlive - 1);
	EnemiesKilled++;

	UE_LOG(LogTemp, Verbose, TEXT("EnemySpawner: Enemy died. Alive=%d, Killed=%d/%d"),
		EnemiesAlive, EnemiesKilled, TotalEnemiesThisWave);

	CheckIfWaveCleared();
}

void AFortEnemySpawner::CheckIfWaveCleared()
{
	if (!bIsSpawningWave && EnemiesAlive <= 0)
	{
		UE_LOG(LogTemp, Log, TEXT("EnemySpawner: Wave %d CLEARED"), CurrentWaveIndex);
		OnWaveCleared.Broadcast(CurrentWaveIndex);
	}
}