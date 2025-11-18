// Fill out your copyright notice in the Description page of Project Settings.


#include "FortTowerBase.h"
#include "AbilitySystemComponent.h"
#include "FortAbilitySystemComponent.h"
#include "FortHealthAttributeSet.h"
#include "FortTowerAttributeSet.h"
#include "Components/SphereComponent.h"
// Sets default values
AFortTowerBase::AFortTowerBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	FortAbilitySystemComp = CreateDefaultSubobject<UFortAbilitySystemComponent>(TEXT("ASC"));
	HealthSet = CreateDefaultSubobject<UFortHealthAttributeSet>(TEXT("HealthSet"));
	TowerAttributeSet = CreateDefaultSubobject<UFortTowerAttributeSet>(TEXT("TowerAttributeSet"));
	AttackRange = CreateDefaultSubobject<USphereComponent>(TEXT("AttackRange"));
}

// Called when the game starts or when spawned
void AFortTowerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFortTowerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFortTowerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent* AFortTowerBase::GetAbilitySystemComponent() const
{
	return  FortAbilitySystemComp;
}