#include "FortEnemyBaseFlyingCharacter.h"

#include "FMODAudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

AFortEnemyBaseFlyingCharacter::AFortEnemyBaseFlyingCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AFortEnemyBaseFlyingCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (UCharacterMovementComponent* MovementComp  = GetCharacterMovement())
	{
		MovementComp->SetMovementMode(MOVE_Flying);
		MovementComp->GravityScale = 0.f;
		MovementComp->MaxFlySpeed = FlightSpeed;
		MovementComp->BrakingDecelerationFlying = FlightSpeed * 2.f;
	}

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(
			SteeringTimerHandle,
			this,
			&AFortEnemyBaseFlyingCharacter::UpdateFlightSteering,
			SteeringInterval,
			true
		);
	}
}

void AFortEnemyBaseFlyingCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(SteeringTimerHandle);
	}

	Super::EndPlay(EndPlayReason);
}

void AFortEnemyBaseFlyingCharacter::SetMoveTarget(AActor* NewTarget)
{
	MoveTargetActor = NewTarget;
}

void AFortEnemyBaseFlyingCharacter::UpdateFlightSteering()
{
	if (!MoveTargetActor)
	{
		return;
	}

	FVector Direction = (MoveTargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	if (Direction.IsNearlyZero())
	{
		return;
	}

	if (bAddOrganicWobble)
	{
		Direction += FMath::VRand() * WobbleStrength;
		Direction = Direction.GetSafeNormal();
	}

	AddMovementInput(Direction, 1.0f);

	// Optional: face movement direction
	SetActorRotation(Direction.Rotation());
}