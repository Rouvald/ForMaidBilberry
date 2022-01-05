// For Maid Bilberry Game. All Rights Recerved

#include "Weapon/FMBBaseWeapon.h"
#include "DrawDebugHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FMBBaseCharacter.h"
#include "Weapon/Components/FMBWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "FMBCoreTypes.h"
#include "Sound/SoundCue.h"

DECLARE_LOG_CATEGORY_CLASS(BaseWeaponLog, All, All);

AFMBBaseWeapon::AFMBBaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    DefaultRootComponent = CreateDefaultSubobject<USceneComponent>("DefaultRootComponent");
    RootComponent = DefaultRootComponent;

    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");
    WeaponMesh->SetupAttachment(DefaultRootComponent);
    WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

    WeaponFXComponent = CreateDefaultSubobject<UFMBWeaponFXComponent>("WeaponFXComponent");

    SwordTrailFXComponent = CreateDefaultSubobject<UNiagaraComponent>("SwordTrailFXComponent");
    SwordTrailFXComponent->SetupAttachment(WeaponMesh, SwordTrailSocketName);
}

void AFMBBaseWeapon::BeginPlay()
{
    Super::BeginPlay();
    check(WeaponMesh);
    check(WeaponFXComponent);

    ChooseDamageAmount.Add(EChooseAttack::FastAttack, FastAttackDamage);
    ChooseDamageAmount.Add(EChooseAttack::StrongAttack, StrongAttackDamage);

    SwordTrailFXComponent->Deactivate();
}

void AFMBBaseWeapon::MeleeAttack(EChooseAttack ChooseAttack)
{
    if (!GetWorld()) return;

    if (ChooseDamageAmount.Contains(ChooseAttack))
    {
        DamageAmount = ChooseDamageAmount[ChooseAttack];
    }

    StartDrawTrace();
}

AController* AFMBBaseWeapon::GetController() const
{
    const auto Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
}

void AFMBBaseWeapon::DrawTrace()
{
    if (!GetWorld()) return;

    const FVector TraceStart = FindBladeSocketLocation(StartBladeTraceSocketName);
    const FVector TraceEnd = FindBladeSocketLocation(EndBladeTraceSocketName);
    if (TraceStart.IsZero() || TraceEnd.IsZero()) return;

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    // DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 0.05f, 0, 1.0f);
    // DrawDebugCylinder(GetWorld(), TraceStart, TraceEnd, TraceRadius, 16, FColor::Purple, false, 0.05f, 0, 1.0f);

    if (HitResult.bBlockingHit)
    {
        SortEqualCharacter(HitResult);
    }
}

FVector AFMBBaseWeapon::FindBladeSocketLocation(FName BladeTraceSocketName) const
{
    const FTransform BladeSocketTransform = WeaponMesh->GetSocketTransform(BladeTraceSocketName);
    const FVector BladeTrace = BladeSocketTransform.GetLocation();
    return BladeTrace;
}

void AFMBBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const
{
    if (!GetWorld()) return;

    const FCollisionShape ColCapsule = FCollisionShape::MakeSphere(TraceRadius);
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    CollisionParams.bReturnPhysicalMaterial = true;

    GetWorld()->SweepSingleByChannel(HitResult, //
        TraceStart,                             //
        TraceEnd,                               //
        FQuat::Identity,                        //
        ECollisionChannel::ECC_Visibility,      //
        ColCapsule,                             //
        CollisionParams                         //
    );
    // GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

void AFMBBaseWeapon::SortEqualCharacter(FHitResult& HitResult)
{
    if (HitActors.Num() == 0)
    {
        NewDamagedActor(HitResult);
        WeaponFXComponent->PlayImpactFX(HitResult);
    }
    else
    {
        bool CheckNewHitActor = false;
        for (const auto HitActor : HitActors)
        {
            if (HitActor == Cast<AActor>(HitResult.GetActor()))
            {
                CheckNewHitActor = true;
            }
        }
        if (!CheckNewHitActor)
        {
            NewDamagedActor(HitResult);
            WeaponFXComponent->PlayImpactFX(HitResult);
        }
    }
}

void AFMBBaseWeapon::NewDamagedActor(FHitResult& HitResult)
{
    HitActors.Add(Cast<AActor>(HitResult.GetActor()));
    if (HitResult.bBlockingHit)
    {
        MakeDamage(HitResult);
        // UE_LOG(BaseWeaponLog, Display, TEXT("Hit %s"), *(Cast<ACharacter>(HitResult.GetActor()))->GetName());
        // DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 20.0f, 32, FColor::Green, false, 5.0f);
    }
}

void AFMBBaseWeapon::MakeDamage(FHitResult& HitResult)
{
    const auto DamagedActor = HitResult.GetActor();
    if (!DamagedActor) return;

    DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetController(), this);
}

void AFMBBaseWeapon::StartDrawTrace()
{
    if (!SwordTrailFXComponent->IsActive())
    {
        SwordTrailFXComponent->Activate(true);
    }
    SwordTrailFXComponent->SetVisibility(true, true);
    SpawnSwordSlashSound();

    // if u wanna use SwordTrails anim notify state -> u need uncomment this down line
    // SwordTrailFXComponent->SetVisibility(false);

    GetWorldTimerManager().SetTimer(DrawTraceTimerHandle, this, &AFMBBaseWeapon::DrawTrace, 0.1f, true);
}

void AFMBBaseWeapon::StopDrawTrace()
{
    HitActors.Empty();

    GetWorldTimerManager().ClearTimer(DrawTraceTimerHandle);
    // SwordTrailFXComponent->Deactivate();
    SwordTrailFXComponent->SetVisibility(false, true);
}

void AFMBBaseWeapon::SpawnSwordSlashSound() const
{
    UGameplayStatics::SpawnSoundAttached(SwordSlashSound, //
        WeaponMesh,                                       //
        SwordTrailSocketName,                             //
        FVector::ZeroVector,                              //
        FRotator::ZeroRotator,                            //
        EAttachLocation::SnapToTarget,                    //
        true);
}
