// Fill out your copyright notice in the Description page of Project Settings.


#include "FortDrillCore.h"
#include "AbilitySystemComponent.h"
#include "FortAbilitySystemComponent.h"
#include "FortHealthAttributeSet.h"
// Sets default values
AFortDrillCore::AFortDrillCore()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DrillMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DrillMesh"));
	DrillMesh->SetupAttachment(RootComponent);
	FortAbilitySystemComp = CreateDefaultSubobject<UFortAbilitySystemComponent>(TEXT("ASC"));
	HealthSet = CreateDefaultSubobject<UFortHealthAttributeSet>(TEXT("HealthSet"));
}

// Called when the game starts or when spawned
void AFortDrillCore::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFortDrillCore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UAbilitySystemComponent* AFortDrillCore::GetAbilitySystemComponent() const
{
	return  FortAbilitySystemComp;
}
