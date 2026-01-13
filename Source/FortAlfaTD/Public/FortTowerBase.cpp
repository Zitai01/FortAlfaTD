// Fill out your copyright notice in the Description page of Project Settings.


#include "FortTowerBase.h"
#include "AbilitySystemComponent.h"
#include "FortAbilitySystemComponent.h"
#include "FortEnemyBaseCharacter.h"
#include "FortHealthAttributeSet.h"
#include "FortTowerAttributeSet.h"
#include "Components/SphereComponent.h"
#include "Data/FortAbilityAsset.h"
// Sets default values
AFortTowerBase::AFortTowerBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;
	MountMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MountMesh"));
	MountMesh->SetupAttachment(BaseMesh,FName(TEXT("Mount_Top")));
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(MountMesh,FName(TEXT("Mount_Weapon_R")));
	FortAbilitySystemComp = CreateDefaultSubobject<UFortAbilitySystemComponent>(TEXT("ASC"));
	HealthSet = CreateDefaultSubobject<UFortHealthAttributeSet>(TEXT("HealthSet"));
	TowerAttributeSet = CreateDefaultSubobject<UFortTowerAttributeSet>(TEXT("TowerAttributeSet"));
	AttackRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackRangeSphere"));
	
	AttackRangeSphere->SetupAttachment(RootComponent);
	float range = TowerAttributeSet->GetAttackRange();
	AttackRangeSphere->InitSphereRadius(range);
	AttackRangeSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttackRangeSphere->SetCollisionObjectType(ECC_WorldDynamic);
	AttackRangeSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackRangeSphere->SetGenerateOverlapEvents(true);
	AttackRangeSphere->SetCollisionResponseToAllChannels(ECR_Ignore);


	
	
}

// Called when the game starts or when spawned
void AFortTowerBase::BeginPlay()
{
	Super::BeginPlay();
	
	AttackRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &AFortTowerBase::OnEnemyEnterRange);
	AttackRangeSphere->OnComponentEndOverlap.AddDynamic(this, &AFortTowerBase::OnEnemyExitRange);
	GetWorld()->GetTimerManager().SetTimer(TowerLogicTimerHandle, this, &AFortTowerBase::TowerUpdate,0.05f,true);
	if (BaseMesh->DoesSocketExist("Mount_Top") && MountMesh->GetStaticMesh())
	{
		MountMesh->AttachToComponent(
			BaseMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, "Mount_Top");
	}
	if (MountMesh->DoesSocketExist("Mount_Weapon_R") && TurretMesh->GetStaticMesh())
	{
		TurretMesh->AttachToComponent(
			MountMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, "Mount_Weapon_R");
	}

	FortAbilitySystemComp->InitAbilityActorInfo(this, this);
	for (TSubclassOf<UGameplayAbility> ability : AbilitySet->Abilities )
	{
		if (FortAbilitySystemComp)
		{
			FGameplayAbilitySpec AbilitySpec(ability, 1, INDEX_NONE, this);
			FortAbilitySystemComp->GiveAbility(AbilitySpec);
		}
	}
	
	for (TSubclassOf<UGameplayEffect> effect : AbilitySet->Effects )
	{
		if (FortAbilitySystemComp)
		{
			UGameplayEffect* effectInstance = NewObject<UGameplayEffect>(this, effect.Get());
			FGameplayEffectContextHandle Context = FortAbilitySystemComp->MakeEffectContext();
			FortAbilitySystemComp->ApplyGameplayEffectToSelf(effectInstance,1,Context);
		}
	}
}

// Called to bind functionality to input
void AFortTowerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFortTowerBase::TowerUpdate()
{
	float DeltaTime = 0.05f;
	
	if (!IsEnemyValid(CurrentTarget))
	{
		CurrentTarget = FindNearestEnemy();
		if (!IsEnemyValid(CurrentTarget))
			return;
	}
	PredictTargetLocation(GetTowerAttributes()->GetProjectileSpeed());
	RotateToFaceEnemy(DeltaTime);
	TryShoot(DeltaTime);
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
	if (AFortEnemyBaseCharacter* Enemy = Cast<AFortEnemyBaseCharacter>(OtherActor))
	{
		EnemiesWithInRange.Add(Enemy);
	}
}

void AFortTowerBase::OnEnemyExitRange(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (AFortEnemyBaseCharacter* Enemy = Cast<AFortEnemyBaseCharacter>(OtherActor))
	{
		EnemiesWithInRange.Remove(Enemy);
		if (CurrentTarget == Enemy)
		{
			CurrentTarget = nullptr; // force re-select next Tick
		}
	}
}

void AFortTowerBase::PredictTargetLocation( float ProjectileSpeed) 
{
	if (!CurrentTarget || ProjectileSpeed <= 0.f)
		TargetPredictedLocation =  FVector::ZeroVector;
	FVector P0 = TurretMesh->GetSocketLocation("Barrel_End");
	FVector P1 = CurrentTarget->GetActorLocation();
	FVector V  = CurrentTarget->GetVelocity();

	FVector D = P1 - P0;

	float A = V.SizeSquared() - FMath::Square(ProjectileSpeed);
	float B = 2.f * FVector::DotProduct(V, D);
	float C = D.SizeSquared();

	float Disc = B * B - 4.f * A * C;
	if (Disc < 0.f)
		TargetPredictedLocation = P1;

	float SqrtDisc = FMath::Sqrt(Disc);
	float T1 = (-B + SqrtDisc) / (2.f * A);
	float T2 = (-B - SqrtDisc) / (2.f * A);

	float T = FMath::Min(T1, T2);
	if (T < 0.f)
		T = FMath::Max(T1, T2);
	if (T < 0.f)
		TargetPredictedLocation = P1;

	TargetPredictedLocation =  P1 + V * T;
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
	if (!Cast<AFortEnemyBaseCharacter>(Enemy))
	{
		return false;
	}
	return true;
}

AFortEnemyBaseCharacter* AFortTowerBase::FindNearestEnemy()
{
	AFortEnemyBaseCharacter* Nearest = nullptr;
	float Closest = FLT_MAX;

	for (int32 i = EnemiesWithInRange.Num() - 1; i >= 0; --i)
	{
		AFortEnemyBaseCharacter* Enemy = EnemiesWithInRange[i];

		if (!IsEnemyValid(Enemy))
		{
			EnemiesWithInRange.RemoveAt(i);
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

	FVector Dir = TargetPredictedLocation - GetActorLocation();
	//Dir.Z = 0;

	FRotator DesiredRot = Dir.Rotation();

	float Speed = 200;

	FRotator NewRot = FMath::RInterpTo(
		GetActorRotation(),
		DesiredRot,
		DeltaTime,
		Speed
	);
	NewRot.Yaw -= 90.0f;
	MountMesh->SetWorldRotation(NewRot);
}

void AFortTowerBase::TryShoot(float DeltaTime)
{
	TimeSinceLastShot += DeltaTime;

	float Interval = 1 / TowerAttributeSet->GetAttackSpeed();
	if (TimeSinceLastShot < Interval)
		return;

	TimeSinceLastShot = 0.f;
	if (auto ASC = CurrentTarget->GetAbilitySystemComponent())
	{
		if (ASC && ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Death.Start")))
		{
			CurrentTarget = nullptr;
			return;
		}
	}

	
	if (FortAbilitySystemComp)
	{
		const FGameplayTag TagToUse = PrimaryAttackTag.IsValid()
	? PrimaryAttackTag
	: FGameplayTag::RequestGameplayTag(FName("Abilities.Skill.ShootBullet"));
		
		if (bIsChanneledAttack)
		{
			// If already active, do NOT re-trigger
			TArray<FGameplayAbilitySpec*> MatchingSpecs;
			FortAbilitySystemComp->GetActivatableGameplayAbilitySpecsByAllMatchingTags(
				TagToUse.GetSingleTagContainer(), MatchingSpecs, /*bOnlyAbilitiesThatSatisfyTagRequirements*/ false
			);

			for (FGameplayAbilitySpec* Spec : MatchingSpecs)
			{
				if (Spec && Spec->IsActive())
				{
					return; // channel already running
				}
				UE_LOG(LogTemp, Verbose, TEXT("[Tower:%s] Channel check: %s specs=%d"),
	*GetName(), *TagToUse.ToString(), MatchingSpecs.Num());

			}
		}
		
		FortAbilitySystemComp->TryActivateAbilitiesByTag(TagToUse.GetSingleTagContainer());
	}
}