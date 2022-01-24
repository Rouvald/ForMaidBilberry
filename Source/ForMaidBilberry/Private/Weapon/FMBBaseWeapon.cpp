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

DECLARE_LOG_CATEGORY_CLASS(LogFMBBaseWeapon, All, All);

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
    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    if (HitResult.bBlockingHit)
    {
        SortEqualCharacter(HitResult);
    }
    // UE_LOG(LogFMBBaseWeapon, Display, TEXT("%s: timer check"), *GetOwner()->GetName());
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
    CollisionParams.AddIgnoredActor(this);
    CollisionParams.bReturnPhysicalMaterial = true;

    GetWorld()->SweepSingleByChannel(HitResult, //
        TraceStart,                             //
        TraceEnd,                               //
        FQuat::Identity,                        //
        ECollisionChannel::ECC_Visibility,      //
        ColCapsule,                             //
        CollisionParams                         //
    );
    // DrawDebugCylinder(GetWorld(), TraceStart, TraceEnd, TraceRadius, 12, FColor::Purple, false, 0.1f);
    // GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

void AFMBBaseWeapon::SortEqualCharacter(const FHitResult& HitResult)
{
    if (!HitActors.Contains(HitResult.GetActor()))
    {
        NewDamagedActor(HitResult);
        WeaponFXComponent->PlayImpactFX(HitResult);
    }
}

void AFMBBaseWeapon::NewDamagedActor(const FHitResult& HitResult)
{
    HitActors.AddUnique(Cast<AActor>(HitResult.GetActor()));
    if (HitResult.bBlockingHit)
    {
        MakeDamage(HitResult);
        // UE_LOG(BaseWeaponLog, Display, TEXT("Hit %s"), *(Cast<ACharacter>(HitResult.GetActor()))->GetName());
        // DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 20.0f, 32, FColor::Green, false, 5.0f);
    }
}

void AFMBBaseWeapon::MakeDamage(const FHitResult& HitResult)
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

    if (HitActors.Num() != 0)
    {
        HitActors.Empty();
        UE_LOG(LogFMBBaseWeapon, Warning, TEXT("HitActors not empty"));
    }
    GetWorldTimerManager().SetTimer(DrawTraceTimerHandle, this, &AFMBBaseWeapon::DrawTrace, 0.05f, true);
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
