// Fill out your copyright notice in the Description page of Project Settings.


#include "FortEnemyBaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "FortAbilitySystemComponent.h"
#include "FortHealthAttributeSet.h"
#include "Components/SphereComponent.h"
// Sets default values
AFortEnemyBaseCharacter::AFortEnemyBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	FortAbilitySystemComp = CreateDefaultSubobject<UFortAbilitySystemComponent>(TEXT("ASC"));
	HealthSet = CreateDefaultSubobject<UFortHealthAttributeSet>(TEXT("HealthSet"));

	MeleeRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("MeleeRangeSphere"));
	MeleeRangeSphere->SetupAttachment(GetRootComponent());
	MeleeRangeSphere->SetSphereRadius(MeleeRange);
	MeleeRangeSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeleeRangeSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeleeRangeSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

// Called when the game starts or when spawned
void AFortEnemyBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	FortAbilitySystemComp->InitAbilityActorInfo(this,this);
	FGameplayTag EnemyTag = FGameplayTag::RequestGameplayTag(FName("Faction.Enemy"));
	
	if (FortAbilitySystemComp)
	{
		FortAbilitySystemComp->AddLooseGameplayTag(EnemyTag);
	}
	MeleeRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &AFortEnemyBaseCharacter::OnMeleeBeginOverlap);
	MeleeRangeSphere->OnComponentEndOverlap.AddDynamic(this, &AFortEnemyBaseCharacter::OnMeleeEndOverlap);
	if (HealthSet)
	{
		HealthSet->SetMaxHealth(2000);
		HealthSet->SetHealth(2000);
	}
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

void AFortEnemyBaseCharacter::InitEnemyFromData(UFortEnemyDataAsset* DA)
{
	/*
	if (!DA || !ASC) return;

	// Apply tags
	ASC->AddLooseGameplayTags(DA->EnemyTags);

	// Apply startup GE (HP, armor, speed…)
	if (DA->StartupEffect)
	{
		FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
		ASC->ApplyGameplayEffectToSelf(DA->StartupEffect.GetDefaultObject(), 1, Context);
	}

	// Additional manual attribute initialization
	// If not covered by StartupEffect:
	Attributes->SetHealth(DA->MaxHealth);
	Attributes->SetMoveSpeed(DA->MoveSpeed);
	Attributes->SetArmor(DA->Armor);
	*/
}

void AFortEnemyBaseCharacter::OnMeleeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                 bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this)
        return;

    // Only attack things that have an ASC (same check your projectile uses) :contentReference[oaicite:7]{index=7}
    UAbilitySystemComponent* TargetASC = Cast<IAbilitySystemInterface>(OtherActor)->GetAbilitySystemComponent();
    if (!TargetASC)
        return;

    // Don’t attack dead targets (same tag your tower checks) :contentReference[oaicite:8]{index=8}
    if (TargetASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Death.Start")))
        return;

    // If you want “first thing entered” behavior:
    if (!CurrentMeleeTarget.IsValid())
    {
        CurrentMeleeTarget = OtherActor;
        StartMelee();
    }
}

void AFortEnemyBaseCharacter::OnMeleeEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (CurrentMeleeTarget.Get() == OtherActor)
    {
        CurrentMeleeTarget = nullptr;
        StopMelee();
    }
}

void AFortEnemyBaseCharacter::StartMelee()
{
    if (!GetWorld()) return;

    // Attack immediately, then repeat
    TryMeleeAttack();

    GetWorld()->GetTimerManager().SetTimer(
        MeleeTimerHandle,
        this,
        &AFortEnemyBaseCharacter::TryMeleeAttack,
        MeleeInterval,
        true
    );
}

void AFortEnemyBaseCharacter::StopMelee()
{
    if (!GetWorld()) return;
    GetWorld()->GetTimerManager().ClearTimer(MeleeTimerHandle);
}

void AFortEnemyBaseCharacter::TryMeleeAttack()
{
    AActor* Target = CurrentMeleeTarget.Get();
    if (!Target || !DamageGEClass)
    {
        StopMelee();
        return;
    }

    UAbilitySystemComponent* TargetASC = Cast<IAbilitySystemInterface>(Target)->GetAbilitySystemComponent();
    if (!TargetASC)
    {
        StopMelee();
        return;
    }

    if (TargetASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Death.Start")))
    {
        StopMelee();
        CurrentMeleeTarget = nullptr;
        return;
    }

    // Apply damage via GE + SetByCaller (same pattern as projectile) :contentReference[oaicite:9]{index=9}
    FGameplayEffectContextHandle Context = TargetASC->MakeEffectContext();
    Context.AddSourceObject(this);
    Context.AddInstigator(this, GetController());

    FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(DamageGEClass, 1.f, Context);
    if (!SpecHandle.IsValid())
        return;

    FGameplayEffectSpec* Spec = SpecHandle.Data.Get();
    check(Spec);

    Spec->SetSetByCallerMagnitude(
        FGameplayTag::RequestGameplayTag("Data.Default.Damage"),
        MeleeDamage
    );

    TargetASC->ApplyGameplayEffectSpecToSelf(*Spec);
}