// Fill out your copyright notice in the Description page of Project Settings.
#include "FortBuildManagerComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"

UFortBuildManagerComponent::UFortBuildManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UFortBuildManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UFortBuildManagerComponent::StartPlacing(TSubclassOf<AActor> InBuildClass)
{
	if (!GetWorld() || !InBuildClass) return;

	BuildClass = InBuildClass;
	State = EFortBuildState::Placing;

	EnsurePreview();

	// timer-driven updates (cheap, stable)
	GetWorld()->GetTimerManager().SetTimer(
		PlacementTimerHandle, this, &UFortBuildManagerComponent::UpdatePlacement,
		PlacementUpdateInterval, true);
}

void UFortBuildManagerComponent::CancelPlacing()
{
	State = EFortBuildState::Off;
	BuildClass = nullptr;

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(PlacementTimerHandle);
	}

	if (PreviewActor)
	{
		PreviewActor->Destroy();
		PreviewActor = nullptr;
	}
}

bool UFortBuildManagerComponent::ConfirmPlace()
{
	if (State != EFortBuildState::Placing || !BuildClass || !GetWorld()) return false;

	// Validate again at confirm time
	FHitResult Hit;
	if (!TraceBuildSurface(Hit) || !IsValidPlacement(Hit))
		return false;

	AActor* Spawned = GetWorld()->SpawnActor<AActor>(BuildClass, PendingTransform);
	if (!Spawned) return false;

	// Optional: set owner/instigator
	// Spawned->SetOwner(GetOwner());

	return true;
}

void UFortBuildManagerComponent::EnsurePreview()
{
	if (!GetWorld() || !BuildClass) return;

	if (!PreviewActor)
	{
		PreviewActor = GetWorld()->SpawnActor<AActor>(BuildClass, FTransform::Identity);
		if (PreviewActor)
		{
			PreviewActor->SetActorEnableCollision(false);
			PreviewActor->SetReplicates(false);

			// TODO: apply ghost material here later
		}
	}
}

void UFortBuildManagerComponent::UpdatePlacement()
{
	if (State != EFortBuildState::Placing || !BuildClass) return;

	EnsurePreview();

	FHitResult Hit;
	if (!TraceBuildSurface(Hit))
		return;

	ApplyPreviewTransform(Hit);

	// You can add green/red visualization later based on IsValidPlacement(Hit)
}

bool UFortBuildManagerComponent::TraceBuildSurface(FHitResult& OutHit) const
{
	const APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (!PC) return false;

	const bool bHit = PC->GetHitResultUnderCursor(BuildTraceChannel, true, OutHit);
	return bHit && OutHit.bBlockingHit;
}

bool UFortBuildManagerComponent::IsValidPlacement(const FHitResult& Hit) const
{
	// Allow terrain by default
	const UPrimitiveComponent* HitComp = Hit.GetComponent();
	const bool bIsTaggedSurface = HitComp && HitComp->ComponentHasTag(BuildSurfaceTag);

	if (bRequireBuildSurfaceTag && !bIsTaggedSurface)
		return false;

	// Basic overlap check: if your preview has collision shapes, we can overlap-test here later
	// Milestone 1: keep it permissive so you get building working first
	return true;
}

void UFortBuildManagerComponent::ApplyPreviewTransform(const FHitResult& Hit)
{
	const FVector Loc = Hit.ImpactPoint;
	const FRotator Rot(0.f, 0.f, 0.f); // later: face camera yaw, or use player rotation

	PendingTransform = FTransform(Rot, Loc);

	if (PreviewActor)
	{
		PreviewActor->SetActorTransform(PendingTransform);
	}
}
