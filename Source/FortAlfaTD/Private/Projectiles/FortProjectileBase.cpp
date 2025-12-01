// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/FortProjectileBase.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "FortAbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Math/UnitConversion.h"

// Sets default values
AFortProjectileBase::AFortProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = CollisionComponent;
	CollisionComponent->InitSphereRadius(10.f);
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->SetGenerateOverlapEvents(true);
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(RootComponent);
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	ProjectileMovementComp->InitialSpeed = 2000.f;
	ProjectileMovementComp->MaxSpeed = 2000.f;
	ProjectileMovementComp->bRotationFollowsVelocity = true;

	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailFX"));
	NiagaraComp->SetupAttachment(StaticMeshComp); 
	NiagaraComp->SetAutoActivate(true);    
	SetActorEnableCollision(true);
}

// Called when the game starts or when spawned
void AFortProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(
	this, &AFortProjectileBase::HandleImpact
);
	
}

void AFortProjectileBase::HandleImpact( UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner())
		return;

	ApplyDamageTo(OtherActor);

	Destroy();

}

void AFortProjectileBase::ApplyDamageTo(AActor* OtherActor)
{
	if (!OtherActor || !DamageGEClass)
		return;

	// Get the ASC on the target
	UAbilitySystemComponent* TargetASC = OtherActor->FindComponentByClass<UAbilitySystemComponent>();
	if (!TargetASC)
		return; // Target cannot take GAS damage

	// Build the GameplayEffect context
	FGameplayEffectContextHandle Context = TargetASC->MakeEffectContext();
	Context.AddSourceObject(this);        // Projectile as source
	Context.AddInstigator(GetOwner(), nullptr); // Shooter (tower/enemy)

	// Create GameplayEffectSpec from BP GE class
	FGameplayEffectSpecHandle SpecHandle =
		TargetASC->MakeOutgoingSpec(DamageGEClass, 1.0f, Context);

	if (!SpecHandle.IsValid())
		return;

	FGameplayEffectSpec* Spec = SpecHandle.Data.Get();
	check(Spec);

	// SetByCaller: pass damage into Damage attribute
	Spec->SetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag("Data.Damage"),
		Damage                // Positive number (your AttributeSet subtracts it)
	);

	// Apply the GameplayEffect to self (the target)
	TargetASC->ApplyGameplayEffectSpecToSelf(*Spec);

	// (Optional) Debug
	UE_LOG(LogTemp, Warning, TEXT("Projectile applied %f damage to %s"),
		Damage,
		*OtherActor->GetName());
}



