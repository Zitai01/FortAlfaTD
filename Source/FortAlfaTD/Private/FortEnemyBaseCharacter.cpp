// Fill out your copyright notice in the Description page of Project Settings.


#include "FortEnemyBaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "FortAbilitySystemComponent.h"
#include "FortHealthAttributeSet.h"
// Sets default values
AFortEnemyBaseCharacter::AFortEnemyBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	FortAbilitySystemComp = CreateDefaultSubobject<UFortAbilitySystemComponent>(TEXT("ASC"));
	HealthSet = CreateDefaultSubobject<UFortHealthAttributeSet>(TEXT("HealthSet"));
}

// Called when the game starts or when spawned
void AFortEnemyBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	FortAbilitySystemComp->InitAbilityActorInfo(this,this);
}

// Called every frame
void AFortEnemyBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFortEnemyBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent* AFortEnemyBaseCharacter::GetAbilitySystemComponent() const
{
	return  FortAbilitySystemComp;
}