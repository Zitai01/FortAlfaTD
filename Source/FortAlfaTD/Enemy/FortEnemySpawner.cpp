// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/FortEnemySpawner.h"

#include "FortDefaultGameInstance.h"
#include "FortEnemyBaseCharacter.h"
#include "FortEnemySpawnPoint.h"
#include "FortMissionWaveSet.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
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
	LoadWavesFromGameInstance();
	//SetupWavesForMission(Mission, Difficulty);
}

void AFortEnemySpawner::LoadWavesFromGameInstance()
{
	UFortDefaultGameInstance* GI = GetWorld() ? Cast<UFortDefaultGameInstance>(GetWorld()->GetGameInstance()) : nullptr;
	if (!GI)
	{
		UE_LOG(LogTemp, Error, TEXT("[EnemySpawner] GameInstance missing or wrong class"));
		return;
	}

	if (GI->SelectedWaves.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("[EnemySpawner] GI.SelectedWaves is empty (Mission=%d)"),
			GI->CurrentLevel);
		return;
	}
	
	TArray<FSoftObjectPath> PathsToLoad;
	PathsToLoad.Reserve(GI->SelectedWaves.Num());

	for (const TSoftObjectPtr<UFortWaveData>& WaveSoft : GI->SelectedWaves)
	{
		if (WaveSoft.IsNull())
		{
			continue;
		}
		PathsToLoad.Add(WaveSoft.ToSoftObjectPath());
	}

	if (PathsToLoad.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("[EnemySpawner] All SelectedWaves entries were null"));
		return;
	}

	// Async load
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	WavesLoadHandle = Streamable.RequestAsyncLoad(
		PathsToLoad,
		FStreamableDelegate::CreateUObject(this, &AFortEnemySpawner::OnWavesLoaded)
	);

	UE_LOG(LogTemp, Log, TEXT("[EnemySpawner] Async loading %d wave assets..."), PathsToLoad.Num());
}

void AFortEnemySpawner::OnWavesLoaded()
{
	UFortDefaultGameInstance* GI = GetWorld() ? Cast<UFortDefaultGameInstance>(GetWorld()->GetGameInstance()) : nullptr;
	if (!GI)
	{
		return;
	}

	// Convert soft refs -> hard pointers for runtime spawning
	Waves.Empty();
	Waves.Reserve(GI->SelectedWaves.Num());

	for (const TSoftObjectPtr<UFortWaveData>& WaveSoft : GI->SelectedWaves)
	{
		if (UFortWaveData* Wave = WaveSoft.Get()) // now loaded
		{
			Waves.Add(Wave);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("[EnemySpawner] Loaded %d/%d wave assets"), Waves.Num(), GI->SelectedWaves.Num());

	if (Waves.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("[EnemySpawner] No valid waves after load"));
		return;
	}

	// Start wave 0
	StartWave(0);
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
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// Completed this group?
	if (SpawnedInCurrentGroup >= CurrentGroup.Count)
	{
		World->GetTimerManager().ClearTimer(SpawnTimerHandle);
		CurrentGroupIndex++;
		SpawnNextGroup();
		return;
	}

	AFortEnemySpawnPoint* SP = GetSpawnPointForLane(CurrentGroup.LaneIndex);
	if (!SP)
	{
		UE_LOG(LogTemp, Warning, TEXT("[EnemySpawner] No spawn point found for lane %d"), CurrentGroup.LaneIndex);
		// You can choose to skip or fallback. Your GetSpawnPointForLane already falls back to SpawnPoints[0]. :contentReference[oaicite:9]{index=9}
		SP = (SpawnPoints.Num() > 0) ? SpawnPoints[0] : nullptr;
		if (!SP) return;
	}

	const FVector SpawnLoc = SP->GetActorLocation();
	const FRotator SpawnRot = SP->GetActorRotation();

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AFortEnemyBaseCharacter* Spawned = World->SpawnActor<AFortEnemyBaseCharacter>(
		Cast<UClass>(CurrentGroup.EnemyClass.Get()),
		SpawnLoc,
		SpawnRot,
		Params
	);

	if (Spawned)
	{
		EnemiesAlive++;
		SpawnedInCurrentGroup++;

		// Optional: let enemy call RegisterEnemyDeath(this) on death.
		// If you have an OnDied delegate, bind it here.

		UE_LOG(LogTemp, Verbose, TEXT("[EnemySpawner] Spawned %s. Alive=%d"), *GetNameSafe(Spawned), EnemiesAlive);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[EnemySpawner] SpawnActor failed (lane %d)"), CurrentGroup.LaneIndex);
	}

}

void AFortEnemySpawner::StartWave(int32 WaveIndex)
{
	if (WaveIndex < 0 || WaveIndex >= Waves.Num() || !Waves[WaveIndex])
	{
		UE_LOG(LogTemp, Warning, TEXT("[EnemySpawner] Invalid wave index %d"), WaveIndex);
		return;
	}

	// Stop anything currently spawning
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(SpawnTimerHandle);
	}

	CurrentWaveIndex = WaveIndex;
	CurrentGroupIndex = 0;

	EnemiesKilled = 0;
	EnemiesAlive = 0;
	TotalEnemiesThisWave = 0;

	// Pre-calc total enemies for UI
	for (const FFortEnemySpawnInfo& G : Waves[WaveIndex]->SpawnGroups)
	{
		TotalEnemiesThisWave += FMath::Max(0, G.Count);
	}

	bIsSpawningWave = true;
	OnWaveStarted.Broadcast(CurrentWaveIndex);

	UE_LOG(LogTemp, Log, TEXT("[EnemySpawner] StartWave %d (Total=%d)"), CurrentWaveIndex, TotalEnemiesThisWave);

	SpawnNextGroup();
}

void AFortEnemySpawner::StartNextWave()
{
	const int32 Next = (CurrentWaveIndex == INDEX_NONE) ? 0 : (CurrentWaveIndex + 1);
	if (Next < Waves.Num())
	{
		StartWave(Next);
	}
}

void AFortEnemySpawner::SetupWaveState()
{
	SpawnedInCurrentGroup = 0;
	if (!Waves.IsValidIndex(CurrentWaveIndex) || !Waves[CurrentWaveIndex]) return;

	const UFortWaveData* Wave = Waves[CurrentWaveIndex];
	if (!Wave->SpawnGroups.IsValidIndex(CurrentGroupIndex)) return;

	CurrentGroup = Wave->SpawnGroups[CurrentGroupIndex];
}

void AFortEnemySpawner::SpawnNextGroup()
{
	if (!Waves.IsValidIndex(CurrentWaveIndex) || !Waves[CurrentWaveIndex])
	{
		bIsSpawningWave = false;
		CheckIfWaveCleared();
		return;
	}

	const UFortWaveData* Wave = Waves[CurrentWaveIndex];

	// Done with all groups -> stop spawning; now we just wait for EnemiesAlive to hit 0
	if (CurrentGroupIndex >= Wave->SpawnGroups.Num())
	{
		bIsSpawningWave = false;
		UE_LOG(LogTemp, Log, TEXT("[EnemySpawner] Finished spawning wave %d. Waiting for kills..."), CurrentWaveIndex);
		CheckIfWaveCleared();
		return;
	}

	SetupWaveState();

	// Validate group
	if (!CurrentGroup.EnemyClass || CurrentGroup.Count <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[EnemySpawner] Wave %d group %d invalid, skipping"), CurrentWaveIndex, CurrentGroupIndex);
		CurrentGroupIndex++;
		SpawnNextGroup();
		return;
	}

	// Kick off repeated spawns for this group
	if (UWorld* World = GetWorld())
	{
		const float Interval = FMath::Max(0.01f, CurrentGroup.SpawnInterval);
		World->GetTimerManager().SetTimer(
			SpawnTimerHandle,
			this,
			&AFortEnemySpawner::SpawnEnemy,
			Interval,
			true,
			0.0f
		);
	}

	UE_LOG(LogTemp, Log, TEXT("[EnemySpawner] Wave %d Group %d: Count=%d Interval=%.2f Lane=%d"),
		CurrentWaveIndex, CurrentGroupIndex, CurrentGroup.Count, CurrentGroup.SpawnInterval, CurrentGroup.LaneIndex);
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