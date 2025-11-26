// Fill out your copyright notice in the Description page of Project Settings.


#include "FortTowerBase.h"
#include "AbilitySystemComponent.h"
#include "FortAbilitySystemComponent.h"
#include "FortEnemyBaseCharacter.h"
#include "FortHealthAttributeSet.h"
#include "FortTowerAttributeSet.h"
#include "Components/SphereComponent.h"
// Sets default values
AFortTowerBase::AFortTowerBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FortAbilitySystemComp = CreateDefaultSubobject<UFortAbilitySystemComponent>(TEXT("ASC"));
	HealthSet = CreateDefaultSubobject<UFortHealthAttributeSet>(TEXT("HealthSet"));
	TowerAttributeSet = CreateDefaultSubobject<UFortTowerAttributeSet>(TEXT("TowerAttributeSet"));
	AttackRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackRangeSphere"));
	AttackRangeSphere->SetupAttachment(RootComponent);
	AttackRange = TowerAttributeSet->GetAttackRange();
	AttackSpeed = TowerAttributeSet->GetAttackSpeed();
	AttackRangeSphere->InitSphereRadius(AttackRange);
	AttackRangeSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttackRangeSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackRangeSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	
}

// Called when the game starts or when spawned
void AFortTowerBase::BeginPlay()
{
	Super::BeginPlay();
	
	AttackRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &AFortTowerBase::OnEnemyEnterRange);
	AttackRangeSphere->OnComponentEndOverlap.AddDynamic(this, &AFortTowerBase::OnEnemyExitRange);
	GetWorld()->GetTimerManager().SetTimer(TowerLogicTimerHandle, this, &AFortTowerBase::TowerUpdate,0.05f,true);
}

// Called to bind functionality to input
void AFortTowerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFortTowerBase::TowerUpdate()
{
	// 1. Pick/validate current target
	// 2. Rotate toward enemy
	// 3. Fire when angle is right
}

UAbilitySystemComponent* AFortTowerBase::GetAbilitySystemComponent() const
{
	return  FortAbilitySystemComp;
}

void AFortTowerBase::OnEnemyEnterRange(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (APawn* Enemy = Cast<AFortEnemyBaseCharacter>(OtherActor))
	{
		EnemiesInRange.Add(Enemy);
	}
}

void AFortTowerBase::OnEnemyExitRange(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (APawn* Enemy = Cast<AFortEnemyBaseCharacter>(OtherActor))
	{
		EnemiesInRange.Remove(Enemy);
		if (CurrentTarget == Enemy)
		{
			CurrentTarget = nullptr; // force re-select next Tick
		}
	}
}

bool AFortTowerBase::IsEnemyValid(APawn* Enemy)
{
	if (!Enemy)
	{
		return false;
	}
	if (UAbilitySystemComponent* ASC = Enemy->FindComponentByClass<UAbilitySystemComponent>())
	{
		static FGameplayTag DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Death.Start"));
		if (ASC->HasMatchingGameplayTag(DeadTag))
			return false;
	}

	return true;
}

APawn* AFortTowerBase::FindNearestEnemy()
{
	APawn* Nearest = nullptr;
	float Closest = FLT_MAX;

	for (int32 i = EnemiesInRange.Num() - 1; i >= 0; --i)
	{
		APawn* Enemy = EnemiesInRange[i];

		if (!IsEnemyValid(Enemy))
		{
			EnemiesInRange.RemoveAt(i);
			continue;
		}

		float Dist = FVector::Dist(GetActorLocation(), Enemy->GetActorLocation());
		if (Dist < Closest)
		{
			Closest = Dist;
			Nearest = Enemy;
		}
	}

	return Nearest;
}

void AFortTowerBase::RotateToFaceEnemy(float DeltaTime)
{
	if (!CurrentTarget) return;

	FVector Dir = CurrentTarget->GetActorLocation() - GetActorLocation();
	Dir.Z = 0; // Only yaw rotation

	FRotator DesiredRot = Dir.Rotation();

	float Speed = 100;

	FRotator NewRot = FMath::RInterpTo(
		GetActorRotation(),
		DesiredRot,
		DeltaTime,
		Speed
	);

	SetActorRotation(NewRot);
}

void AFortTowerBase::TryShoot(float DeltaTime)
{
	TimeSinceLastShot += DeltaTime;

	float Interval = 1 / TowerAttributeSet->GetAttackSpeed();
	if (TimeSinceLastShot < Interval)
		return;

	TimeSinceLastShot = 0.f;

	if (FortAbilitySystemComp && ShootAbility)
	{
		FortAbilitySystemComp->TryActivateAbilityByClass(ShootAbility);
	}
}