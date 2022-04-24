// For Maid Bilberry Game. All Rights Recerved

#include "Weapon/FMBBaseWeapon.h"
#include "DrawDebugHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FMBBaseCharacter.h"
#include "Weapon/Components/FMBWeaponFXComponent.h"
#include "NiagaraComponent.h"
#include "FMBCoreTypes.h"
#include "FMBUtils.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/FMBItemInfoWidget.h"
#include "Sound/SoundCue.h"

DECLARE_LOG_CATEGORY_CLASS(LogFMBBaseWeapon, All, All);

AFMBBaseWeapon::AFMBBaseWeapon()
{
    /*BoxCollision->SetRelativeLocation(FVector{0.0f, 70.0f, 0.0f});
    BoxCollision->SetBoxExtent(FVector{30.0f, 95.0f, 6.0f});*/

    AreaCollision->SetRelativeLocation(ItemMesh->GetRelativeLocation());

    WeaponFXComponent = CreateDefaultSubobject<UFMBWeaponFXComponent>(TEXT("WeaponFXComponent"));

    SwordTrailFXComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SwordTrailFXComponent"));
    SwordTrailFXComponent->SetupAttachment(ItemMesh);
    SwordTrailFXComponent->SetRelativeLocation(FVector{0.0f, 125.0f, 0.0f});
}

void AFMBBaseWeapon::BeginPlay()
{
    Super::BeginPlay();
    checkf(WeaponFXComponent, TEXT("WeaponFxComponent == nullptr"));

    ChooseDamageAmount.Add(EChooseAttack::ECA_FastAttack, WeaponDamageData.FastAttackDamage);
    ChooseDamageAmount.Add(EChooseAttack::ECA_StrongAttack, WeaponDamageData.StrongAttackDamage);

    SwordTrailFXComponent->Deactivate();
}

void AFMBBaseWeapon::SetItemInfo() const
{
    Super::SetItemInfo();

    const auto ItemInfoWidget{Cast<UFMBItemInfoWidget>(ItemInfoWidgetComponent->GetWidget())};
    if (ItemInfoWidget)
    {
        ItemInfoWidget->SetItemFastAttackDamage(WeaponDamageData.FastAttackDamage);
        ItemInfoWidget->SetItemStrongAttackDamage(WeaponDamageData.StrongAttackDamage);
    }
}

void AFMBBaseWeapon::MeleeAttack(const EChooseAttack ChooseAttack)
{
    if (!GetWorld()) return;

    if (ChooseDamageAmount.Contains(ChooseAttack))
    {
        DamageAmount = ChooseDamageAmount[ChooseAttack];
    }
    StartDrawTrace();
}

void AFMBBaseWeapon::DrawTrace()
{
    if (!GetWorld()) return;

    const FVector TraceStart{FindBladeSocketLocation(StartBladeTraceSocketName)};
    const FVector TraceEnd{FindBladeSocketLocation(EndBladeTraceSocketName)};
    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    if (HitResult.bBlockingHit)
    {
        SortEqualCharacter(HitResult);
    }
}

FVector AFMBBaseWeapon::FindBladeSocketLocation(const FName BladeTraceSocketName) const
{
    const FTransform BladeSocketTransform = ItemMesh->GetSocketTransform(BladeTraceSocketName);
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
    if (HitActors.Contains(HitResult.GetActor())) return;

    const auto OwnerPawn = Cast<APawn>(GetOwner());
    const auto HittedActor = Cast<APawn>(HitResult.GetActor());

    if (!OwnerPawn) return;

    /* @note: in future we can add AActor that can be broke ( like wooden chest ) */
    if (!HittedActor)
    {
        NewDamagedActor(HitResult);
        WeaponFXComponent->PlayImpactFX(HitResult);
        return;
    }
    if (FMBUtils::AreBothBots(OwnerPawn->GetController(), HittedActor->GetController())) return;

    NewDamagedActor(HitResult);
    WeaponFXComponent->PlayImpactFX(HitResult);
}

void AFMBBaseWeapon::NewDamagedActor(const FHitResult& HitResult)
{
    HitActors.AddUnique(HitResult.GetActor());
    MakeDamage(HitResult);
    // UE_LOG(BaseWeaponLog, Display, TEXT("Hit %s"), *(Cast<ACharacter>(HitResult.GetActor()))->GetName());
    // DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 20.0f, 32, FColor::Green, false, 5.0f);
}

void AFMBBaseWeapon::MakeDamage(const FHitResult& HitResult)
{
    const auto DamagedActor = HitResult.GetActor();
    if (!DamagedActor) return;

    DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetController(), this);
}

void AFMBBaseWeapon::StartDrawTrace()
{
    SwordTrailFXComponent->Activate(true);

    SpawnSwordSlashSound();

    if (HitActors.Num() != 0)
    {
        HitActors.Empty();
        UE_LOG(LogFMBBaseWeapon, Warning, TEXT("HitActors not empty"));
    }
    GetWorldTimerManager().SetTimer(DrawTraceTimerHandle, this, &AFMBBaseWeapon::DrawTrace, 0.01f, true);
}

void AFMBBaseWeapon::StopDrawTrace()
{
    HitActors.Empty();

    GetWorldTimerManager().ClearTimer(DrawTraceTimerHandle);
    SwordTrailFXComponent->Deactivate();
}

void AFMBBaseWeapon::SpawnSwordSlashSound() const
{
    UGameplayStatics::SpawnSoundAttached(SwordSlashSound, //
        ItemMesh,                                         //
        SwordTrailSocketName,                             //
        FVector::ZeroVector,                              //
        FRotator::ZeroRotator,                            //
        EAttachLocation::SnapToTarget,                    //
        true);
}