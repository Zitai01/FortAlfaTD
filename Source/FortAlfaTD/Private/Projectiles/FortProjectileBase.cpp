// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/FortProjectileBase.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Math/UnitConversion.h"

// Sets default values
AFortProjectileBase::AFortProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovementComp"));
	
}

// Called when the game starts or when spawned
void AFortProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}


