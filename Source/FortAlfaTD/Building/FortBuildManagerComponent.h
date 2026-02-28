// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FortBuildManagerComponent.generated.h"

UENUM(BlueprintType)
enum class EFortBuildState : uint8
{
	Off,
	Placing
};

UCLASS(ClassGroup=(Fort), meta=(BlueprintSpawnableComponent))
class FORTALFATD_API UFortBuildManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFortBuildManagerComponent();
	
	UFUNCTION(BlueprintCallable, Category="Build")
	void StartPlacing(TSubclassOf<AActor> InBuildClass);

	UFUNCTION(BlueprintCallable, Category="Build")
	void CancelPlacing();

	UFUNCTION(BlueprintCallable, Category="Build")
	bool ConfirmPlace();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Build")
	float PlacementUpdateInterval = 0.03f; // ~33Hz feels smooth

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Build")
	float SurfaceTagSnapRadius = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Build")
	bool bRequireBuildSurfaceTag = false; // false = allow terrain too

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Build")
	FName BuildSurfaceTag = "BuildSurface";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Build")
	TEnumAsByte<ECollisionChannel> BuildTraceChannel = ECC_Visibility;

protected:
	virtual void BeginPlay() override;

private:
	void UpdatePlacement();
	bool TraceBuildSurface(FHitResult& OutHit) const;
	bool IsValidPlacement(const FHitResult& Hit) const;
	void ApplyPreviewTransform(const FHitResult& Hit);
	void EnsurePreview();

private:
	EFortBuildState State = EFortBuildState::Off;

	UPROPERTY()
	TSubclassOf<AActor> BuildClass;

	UPROPERTY()
	TObjectPtr<AActor> PreviewActor;

	FTransform PendingTransform;
	FTimerHandle PlacementTimerHandle;
};