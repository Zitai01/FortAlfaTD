// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/FortEnemySpawner.h"

#include "FortDefaultGameInstance.h"
#include "FortEnemyBaseCharacter.h"
#include "FortEnemySpawnPoint.h"
#include "FortMissionWaveSet.h"
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
	
	UFortDefaultGameInstance* GI = Cast<UFortDefaultGameInstance>(GetGameInstance());
	if (!GI)
	{
		UE_LOG(LogTemp, Error, TEXT("Spawner: GameInstance not found!"));
		return;
	}
	int32 Mission = GI->CurrentLevel;
	int32 Difficulty = 1;
	SetupWavesForMission(Mission, Difficulty);
	GetWorld()->GetTimerManager().SetTimer(
	SpawnTimerHandle,
	this,
	&AFortEnemySpawner::SpawnEnemy,
	20,
	true,
	1
);
}

void AFortEnemySpawner::SetupWavesForMission(int32 Mission, int32 Difficulty)
{
	Waves.Empty();

	UFortMissionWaveSet* FoundSet = nullptr;
	for (UFortMissionWaveSet* Set : MissionWaveSets)
	{
		if (Set && Set->MissionId == Mission)
		{
			FoundSet = Set;
			break;
		}
	}

	if (!FoundSet)
	{
		UE_LOG(LogTemp, Error, TEXT("[EnemySpawner] No MissionWaveSet found for Mission=%d"), Mission);
		return;
	}

	// Difficulty mapping: 0=Easy,1=Normal,2=Hard (adjust to your preference)
	switch (Difficulty)
	{
	case 0: Waves = FoundSet->Waves_Easy;   break;
	case 2: Waves = FoundSet->Waves_Hard;   break;
	case 1:
	default: Waves = FoundSet->Waves_Normal; break;
	}

	UE_LOG(LogTemp, Log, TEXT("[EnemySpawner] SetupWavesForMission Mission=%d Difficulty=%d -> Waves=%d"),
		Mission, Difficulty, Waves.Num());
}

void AFortEnemySpawner::FindSpawnPointsInLevel()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFortEnemySpawnPoint::StaticClass(), FoundActors);

	SpawnPoints.Empty();
	for (AActor* Actor : FoundActors)
	{
		if (AFortEnemySpawnPoint* SP = Cast<AFortEnemySpawnPoint>(Actor))
		{
			SpawnPoints.Add(SP);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("[EnemySpawner] Found %d spawn points"), SpawnPoints.Num());
}

void AFortEnemySpawner::SpawnEnemy()
{
	// Stop if we reached max
	if (SpawnedCount >= 10)
	{
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(SpawnTimerHandle);
		}

		UE_LOG(LogTemp, Log, TEXT("[EnemySpawner] Reached MaxSpawnCount=%d, stopped."), 10);
		return;
	}

	UWorld* World = GetWorld();
	if (!World) return;

	const FVector SpawnLoc =  SpawnPoints[0]->GetActorLocation();
	const FRotator SpawnRot =  SpawnPoints[0]->GetActorRotation();

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	 for (int32 i = 0; i < 10; i++)
	 {
	 	AActor* Spawned = World->SpawnActor<AFortEnemyBaseCharacter>(EnemyClass, SpawnLoc, SpawnRot, Params);
	 	if (Spawned)
	 	{
	 		SpawnedCount++;
	 		UE_LOG(LogTemp, Verbose, TEXT("[EnemySpawner] Spawned %s (%d)"), *GetNameSafe(Spawned), SpawnedCount);
	 	}
	 }

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