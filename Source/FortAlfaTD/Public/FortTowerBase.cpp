// Fill out your copyright notice in the Description page of Project Settings.


#include "FortTowerBase.h"
#include "AbilitySystemComponent.h"
#include "FortAbilitySystemComponent.h"
#include "FortEnemyBaseCharacter.h"
#include "FortHealthAttributeSet.h"
#include "FortTowerAttributeSet.h"
#include "TowerData.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Data/FortAbilityAsset.h"
#include "ActiveGameplayEffectHandle.h"
#include "FortPlayerState.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
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

	RotatingAudioComp = CreateDefaultSubobject<UAudioComponent>("RotatingAudioComp");
	RotatingAudioComp->SetupAttachment(RootComponent);
	RotatingAudioComp->bAutoActivate = false;
	RotatingAudioComp->bAllowSpatialization = true;
	
	LaserFiringStateTag = FGameplayTag::RequestGameplayTag(TEXT("State.Firing.Laser"));
	LaserAbilityTag     = FGameplayTag::RequestGameplayTag(TEXT("Abilities.Skill.LaserBeam"));
}

// Called when the game starts or when spawned
void AFortTowerBase::BeginPlay()
{
	Super::BeginPlay();
	
	AttackRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &AFortTowerBase::OnEnemyEnterRange);
	AttackRangeSphere->OnComponentEndOverlap.AddDynamic(this, &AFortTowerBase::OnEnemyExitRange);
	GetWorld()->GetTimerManager().SetTimer(AudioLogicTimerHandle, this, &AFortTowerBase::AudioUpdate,0.5f,true);
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

	if (RotatingAudioComp && BaseRotationMetaSound)
	{
		RotatingAudioComp->SetSound(BaseRotationMetaSound);
	}
	MountRotationYawDeg = MountMesh->GetComponentRotation().Yaw;

	FortAbilitySystemComp->SetNumericAttributeBase(
	UFortTowerAttributeSet::GetAttackDamageAttribute(),
	TowerData->Damage
);

	FortAbilitySystemComp->SetNumericAttributeBase(
		UFortTowerAttributeSet::GetAttackSpeedAttribute(),
		FMath::Max(1.f, TowerData->FireRate)
	);
	
	// Update the sphere radius too (constructor currently uses the attribute default)
	AttackRangeSphere->SetSphereRadius(TowerData->Range, true);
	BindTechListener();
	ReapplyTech();
}

// Called to bind functionality to input
void AFortTowerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFortTowerBase::TowerUpdate()
{
	float DeltaTime = 0.1f;
	
	if (!IsEnemyValid(CurrentTarget))
	{
		CurrentTarget = FindNearestEnemy();
		if (!IsEnemyValid(CurrentTarget))
		{
			if (RotatingAudioComp)
			{
				RotatingAudioComp->Stop();
				bRotationAudioPlaying = false;
				return;
			}
		}
	}
	PredictTargetLocation(GetTowerAttributes()->GetProjectileSpeed());

	if (!HasLineOfSightToTarget(CurrentTarget))
	{
		// If laser tower, stop the channel if LOS breaks
		if (bIsChanneledAttack)
		{
			StopChanneledAttack();
		}
		return;
	}
	RotateToFaceEnemy(DeltaTime);
	if (bIsChanneledAttack)
	{
		// Laser mode: keep ability running while target is valid
		EnsureChanneledAttackActive();

		// IMPORTANT: don't run interval-based TryShoot for channeled towers
		return;
	}
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
			if (bIsChanneledAttack)
			{
				StopChanneledAttack();
			}
			CurrentTarget = nullptr;
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

void AFortTowerBase::EnsureChanneledAttackActive()
{
	if (!FortAbilitySystemComp) return;
	if (!IsEnemyValid(CurrentTarget)) return;
	
	if (FortAbilitySystemComp->HasMatchingGameplayTag(LaserFiringStateTag))
		return;
	
	FGameplayTagContainer AbilityTags;
	AbilityTags.AddTag(LaserAbilityTag);

	FortAbilitySystemComp->TryActivateAbilitiesByTag(AbilityTags);
}

void AFortTowerBase::StopChanneledAttack()
{
	if (!FortAbilitySystemComp) return;

	// Cancel the channeled laser ability (triggers EndAbility -> beam stops)
	FGameplayTagContainer AbilityTags;
	AbilityTags.AddTag(LaserAbilityTag);

	FortAbilitySystemComp->CancelAbilities(&AbilityTags);
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

	const bool bIsUpsideDown =
	FVector::DotProduct(MountMesh->GetUpVector(), FVector::UpVector) < 0.f;
	
	if (!CurrentTarget) return;
	if (!MountMesh || !TurretMesh) return;
	
	const FVector MountLoc  = MountMesh->GetComponentLocation();
	const FVector TurretLoc = TurretMesh->GetComponentLocation();
	
	FVector DirYawWorld = TargetPredictedLocation - MountLoc;
	DirYawWorld.Z = 0.f;
	if (DirYawWorld.IsNearlyZero()) return;

	const FVector AimDirWorld = (TargetPredictedLocation - TurretLoc).GetSafeNormal();
	if (AimDirWorld.IsNearlyZero()) return;


	float DesiredWorldYaw = DirYawWorld.Rotation().Yaw - 90.f;


	const USceneComponent* Parent = MountMesh->GetAttachParent();
	const float ParentWorldYaw = Parent ? Parent->GetComponentRotation().Yaw : GetActorRotation().Yaw;


	float DesiredRelYaw = FMath::FindDeltaAngleDegrees(ParentWorldYaw, DesiredWorldYaw);

	if (bIsUpsideDown)
	{
		DesiredRelYaw *= -1.f;
	}
	FRotator MountRel = MountMesh->GetRelativeRotation();

	const float Speed = 150.f;
	MountRel.Yaw = FMath::FixedTurn(MountRel.Yaw, DesiredRelYaw, Speed * DeltaTime);

	MountMesh->SetRelativeRotation(MountRel);

	
	const FVector AimDirLocalToMount =
		MountMesh->GetComponentTransform().InverseTransformVectorNoScale(AimDirWorld);

	//  forward = +Y
	float DesiredRoll = FMath::RadiansToDegrees(FMath::Atan2(AimDirLocalToMount.Z, AimDirLocalToMount.Y));
	//DesiredRoll = FMath::Clamp(DesiredRoll, -60.f, 80.f);
	DesiredRoll *= -1.f;
	
	FRotator TurretRel = TurretMesh->GetRelativeRotation();
	TurretRel.Roll = FMath::FixedTurn(TurretRel.Roll, DesiredRoll, Speed * DeltaTime);
	TurretMesh->SetRelativeRotation(TurretRel);


	// Rotation Audio 
	const float CurrentWorldYaw = MountMesh->GetComponentRotation().Yaw;
	const float DeltaYaw = FMath::FindDeltaAngleDegrees(MountRotationYawDeg, CurrentWorldYaw);
	const float YawRotationPerSec = FMath::Abs(DeltaYaw) / FMath::Max(DeltaTime, KINDA_SMALL_NUMBER);
	MountRotationYawDeg = CurrentWorldYaw;
	/*
	if (RotatingAudioComp)
	{
		const bool bShouldStart = (YawRotationPerSec >= RotationStartThresholdDegPerSec);
		const bool bShouldStop  = (YawRotationPerSec <= RotationStopThresholdDegPerSec);

		if (bShouldStart && !bRotationAudioPlaying)
		{
			bRotationAudioPlaying = true;
			RotatingAudioComp->Play();
		}
		else if (bShouldStop && bRotationAudioPlaying)
		{
			bRotationAudioPlaying = false;
			RotatingAudioComp->Stop();
		}

		RotatingAudioComp->SetFloatParameter(TEXT("RotationSpeed"), YawRotationPerSec);
		
	}
	*/
}


void AFortTowerBase::TryShoot(float DeltaTime)
{
	TimeSinceLastShot += DeltaTime;

	float Interval = 1/ TowerAttributeSet->GetAttackSpeed();
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

void AFortTowerBase::AudioUpdate()
{

}
bool AFortTowerBase::HasLineOfSightToTarget(AFortEnemyBaseCharacter* Target) const
{
	if (!bRequireLineOfSight) return true;
	if (!Target || !TurretMesh) return false;
	if (!GetWorld()) return false;

	// Start from muzzle (same socket you're already using for projectile prediction)
	FVector Start = TurretMesh->DoesSocketExist("Barrel_End")
		? TurretMesh->GetSocketLocation("Barrel_End")
		: TurretMesh->GetComponentLocation();

	// End at enemy "center-ish"
	FVector End = Target->GetActorLocation();
	End.Z += TargetAimZOffset;

	// Optional: if target has a capsule, aim at mid-body instead
	if (const UCapsuleComponent* Cap = Target->FindComponentByClass<UCapsuleComponent>())
	{
		End = Target->GetActorLocation();
		End.Z += Cap->GetScaledCapsuleHalfHeight() * 0.5f;
	}

	FHitResult Hit;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(TowerLOS), true);
	Params.AddIgnoredActor(this);

	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		Start,
		End,
		LineOfSightTraceChannel,
		Params
	);

	// If nothing blocks, we can see it
	if (!bHit)
		return true;

	// If first thing hit IS the target, we can see it
	return Hit.GetActor() == Target;
}

void AFortTowerBase::BindTechListener()
{
	CachedOwnerPS = ResolveFortPlayerState();

	if (CachedOwnerPS.IsValid())
	{
		CachedOwnerPS->OnTechChanged.AddUObject(this, &AFortTowerBase::HandleTechChanged);
	}
}

void AFortTowerBase::UnbindTechListener()
{
	if (CachedOwnerPS.IsValid())
	{
		CachedOwnerPS->OnTechChanged.RemoveAll(this);
	}
}

void AFortTowerBase::HandleTechChanged()
{
	ReapplyTech();
}

void AFortTowerBase::ReapplyTech()
{
	if (!FortAbilitySystemComp) return;
	if (!CachedOwnerPS.IsValid()) return;
	if (!TowerData) return;

	// Remove previous tech GE so it doesn't stack forever
	if (TechGEHandle.IsValid())
	{
		FortAbilitySystemComp->RemoveActiveGameplayEffect(TechGEHandle);
		TechGEHandle.Invalidate();
	}

	TechGEHandle = CachedOwnerPS->ApplyTechToTowerASC(FortAbilitySystemComp, TowerData);

	// IMPORTANT: if tech modifies AttackRange, refresh the sphere radius from the attribute value
	if (AttackRangeSphere && TowerAttributeSet)
	{
		AttackRangeSphere->SetSphereRadius(TowerAttributeSet->GetAttackRange(), true);
	}
}

void AFortTowerBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnbindTechListener();
	Super::EndPlay(EndPlayReason);
}

AFortPlayerState* AFortTowerBase::ResolveFortPlayerState() const
{
    // If tower actually owned by player later (build system), use it
    if (const APlayerController* PC = Cast<APlayerController>(GetOwner()))
        return Cast<AFortPlayerState>(PC->PlayerState);

    // If owned by a pawn/character
    if (const APawn* P = Cast<APawn>(GetOwner()))
        return Cast<AFortPlayerState>(P->GetPlayerState());

    // MVP fallback for placed towers / AIController owner
    if (APlayerController* PC0 = UGameplayStatics::GetPlayerController(this, 0))
        return Cast<AFortPlayerState>(PC0->PlayerState);

    return nullptr;
}