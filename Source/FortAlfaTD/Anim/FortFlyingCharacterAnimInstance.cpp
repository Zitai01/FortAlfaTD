// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/FortFlyingCharacterAnimInstance.h"

#include "Enemy/FortEnemyBaseFlyingCharacter.h"

void UFortFlyingCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* Pawn = TryGetPawnOwner();
	if (!Pawn) return;

	AFortEnemyBaseFlyingCharacter* FlyingCharacter = Cast<AFortEnemyBaseFlyingCharacter>(Pawn);
	if (!FlyingCharacter) return;

	Speed         = FlyingCharacter->Speed;
	VerticalSpeed = FlyingCharacter->VerticalSpeed;
	bIsGrounded   = FlyingCharacter->bIsGrounded;
	bIsFalling    = FlyingCharacter->bIsFalling;
	bIsFlying     = FlyingCharacter->IsFlying();
}
