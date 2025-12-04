#include "Projectiles/FortProjectileBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
AFortProjectileBase::AFortProjectileBase()
{
    PrimaryActorTick.bCanEverTick = true;

    // --- Collision (Root) ---
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    RootComponent = CollisionComponent;
    CollisionComponent->InitSphereRadius(20.f);
    CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
    CollisionComponent->SetGenerateOverlapEvents(true);

    // --- Projectile Movement ---
    ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
    ProjectileMovementComp->InitialSpeed = 1000.f;
    ProjectileMovementComp->MaxSpeed = 1000.f;
    ProjectileMovementComp->bRotationFollowsVelocity = true;
    ProjectileMovementComp->ProjectileGravityScale = 0.f;
    ProjectileMovementComp->SetUpdatedComponent(CollisionComponent);

    // --- Mesh ---
    StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMeshComp->SetupAttachment(RootComponent);
    StaticMeshComp->SetCollisionProfileName(TEXT("Projectile"));

    // --- Niagara Trail ---
    NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailFX"));
    NiagaraComp->SetupAttachment(StaticMeshComp);
    NiagaraComp->SetAutoActivate(true);
    InitialLifeSpan = 10.0f;  
    SetActorEnableCollision(true);
}

// ---------------------------------------------------------------------------
// BeginPlay
// ---------------------------------------------------------------------------
void AFortProjectileBase::BeginPlay()
{
    Super::BeginPlay();

    CollisionComponent->OnComponentBeginOverlap.AddDynamic(
        this, &AFortProjectileBase::HandleImpact
    );
}

// ---------------------------------------------------------------------------
// Tick
// ---------------------------------------------------------------------------
void AFortProjectileBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// ---------------------------------------------------------------------------
// Fire: Apply initial velocity
// ---------------------------------------------------------------------------
void AFortProjectileBase::FireInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComp->Velocity =
        ShootDirection * ProjectileMovementComp->InitialSpeed;
}

// ---------------------------------------------------------------------------
// Overlap → Damage → Destroy
// ---------------------------------------------------------------------------
void AFortProjectileBase::HandleImpact(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (!OtherActor ||
        OtherActor == this ||
        OtherActor == GetOwner() ||
        OtherActor->IsA(AFortProjectileBase::StaticClass()))
    {
        return;
    }

    ApplyDamageTo(OtherActor);
    Destroy();
}

// ---------------------------------------------------------------------------
// Apply GameplayEffect damage
// ---------------------------------------------------------------------------
void AFortProjectileBase::ApplyDamageTo(AActor* OtherActor)
{
    if (!OtherActor || !DamageGEClass)
        return;

    UAbilitySystemComponent* TargetASC =
        OtherActor->FindComponentByClass<UAbilitySystemComponent>();

    if (!TargetASC)
        return;

    FGameplayEffectContextHandle Context = TargetASC->MakeEffectContext();
    Context.AddSourceObject(this);
    Context.AddInstigator(GetOwner(), nullptr);

    FGameplayEffectSpecHandle SpecHandle =
        TargetASC->MakeOutgoingSpec(DamageGEClass, 1.f, Context);

    if (!SpecHandle.IsValid())
        return;

    FGameplayEffectSpec* Spec = SpecHandle.Data.Get();
    check(Spec);

    Spec->SetSetByCallerMagnitude(
        FGameplayTag::RequestGameplayTag("Data.Default.Damage"),
        Damage
    );

    TargetASC->ApplyGameplayEffectSpecToSelf(*Spec);

    UE_LOG(LogTemp, Warning, TEXT("Projectile applied %f damage to %s"),
        Damage,
        *OtherActor->GetName());
}
