// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/FortWaveData.h"
#include "FortEnemySpawner.generated.h"

class AFortEnemySpawnPoint;
class AFortEnemyBaseCharacter;

UCLASS()
class FORTALFATD_API AFortEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFortEnemySpawner();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Waves")
    TArray<UFortWaveData*> Waves;
	
    // Found automatically at BeginPlay
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Spawn")
    TArray<AFortEnemySpawnPoint*> SpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Waves")
	TArray<class UFortMissionWaveSet*> MissionWaveSets;

	// What to spawn test only, delete later
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawning")
	TSubclassOf<AFortEnemyBaseCharacter> EnemyClass;
	
    // Optional difficulty scalar (you can use this in SetupWaveState)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Difficulty")
    float DifficultyMultiplier = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Spawning")
	int32 SpawnedCount = 0;
    // ---- Events ----

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaveStarted, int32, WaveIndex);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaveCleared, int32, WaveIndex);

    // Fired when wave spawns start
    UPROPERTY(BlueprintAssignable, Category="Events")
    FOnWaveStarted OnWaveStarted;

    // Fired when all enemies in this wave are dead AND spawning finished
    UPROPERTY(BlueprintAssignable, Category="Events")
    FOnWaveCleared OnWaveCleared;

    // ---- Public API ----

    // Start a specific wave by index
    UFUNCTION(BlueprintCallable, Category="Waves")
    void StartWave(int32 WaveIndex);

    // Advance to next wave (WaveIndex + 1) if exists
    UFUNCTION(BlueprintCallable, Category="Waves")
    void StartNextWave();

    // Called by enemies when they die
    void RegisterEnemyDeath(AFortEnemyBaseCharacter* DeadEnemy);

    // Get how many enemies are currently alive in the level (for UI)
    UFUNCTION(BlueprintCallable, Category="Waves")
    int32 GetEnemiesAlive() const { return EnemiesAlive; }

    UFUNCTION(BlueprintCallable, Category="Waves")
    int32 GetEnemiesKilledThisWave() const { return EnemiesKilled; }

    UFUNCTION(BlueprintCallable, Category="Waves")
    int32 GetTotalEnemiesThisWave() const { return TotalEnemiesThisWave; }

private:
    // ---- Internal State ----

    int32 CurrentWaveIndex = INDEX_NONE;
    int32 CurrentGroupIndex = 0;

    // The spawn group currently being processed
    FFortEnemySpawnInfo CurrentGroup;
    int32 SpawnedInCurrentGroup = 0;

    // Counters for the whole wave
    int32 EnemiesAlive = 0;
    int32 EnemiesKilled = 0;
    int32 TotalEnemiesThisWave = 0;

    bool bIsSpawningWave = false;

    FTimerHandle SpawnTimerHandle;

    // ---- Internal Methods ----

    void FindSpawnPointsInLevel();
    void SetupWaveState();
    void SpawnNextGroup();
    void SpawnEnemy();

    AFortEnemySpawnPoint* GetSpawnPointForLane(int32 LaneIndex) const;
    void CheckIfWaveCleared();
    void SetupWavesForMission(int32 Mission, int32 Difficulty);
};
