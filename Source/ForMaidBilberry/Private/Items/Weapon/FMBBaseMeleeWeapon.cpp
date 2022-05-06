// For Maid Bilberry Game. All Rights Reserved

#include "Items/Weapon/FMBBaseMeleeWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "Components/WidgetComponent.h"
#include "FMBBaseCharacter.h"
#include "FMBPlayerCharacter.h"
#include "FMBPlayerWeaponComponent.h"
#include "UI/FMBItemInfoWidget.h"
#include "Weapon/Components/FMBWeaponFXComponent.h"
#include "FMBUtils.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBBaseMeleeWeapon, All, All)

AFMBBaseMeleeWeapon::AFMBBaseMeleeWeapon()
{
    ItemInfoWidgetComponent->SetRelativeLocation(FVector{0.0f, 70.0f, 0.0f});

    SwordTrailFXComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SwordTrailFXComponent"));
    SwordTrailFXComponent->SetupAttachment(ItemMesh);
    SwordTrailFXComponent->SetRelativeLocation(FVector{0.0f, 125.0f, 0.0f});
}

void AFMBBaseMeleeWeapon::BeginPlay()
{
    Super::BeginPlay();
    SwordTrailFXComponent->Deactivate();
}

void AFMBBaseMeleeWeapon::UpdateItemInfoProperty(const AFMBPlayerCharacter* CurrentPlayerCharacter) const
{
    if (!CurrentPlayerCharacter) return;

    const auto WeaponComponent{CurrentPlayerCharacter->FindComponentByClass<UFMBPlayerWeaponComponent>()};
    if (!WeaponComponent) return;

    const auto ItemInfoWidget{Cast<UFMBItemInfoWidget>(ItemInfoWidgetComponent->GetWidget())};
    if (!ItemInfoWidget) return;

    ItemInfoWidget->SetItemProperty(
        WeaponComponent->GetCurrentWeapon()
            ? WeaponDamageData.DefaultDamage - WeaponComponent->GetCurrentWeapon()->GetWeaponDamageData().DefaultDamage
            : WeaponDamageData.DefaultDamage);
}

void AFMBBaseMeleeWeapon::MeleeAttack(const EChooseAttack ChooseAttack)
{
    if (!GetWorld()) return;
    DamageAmount = ChooseDamageAmount.FindChecked(ChooseAttack);
    StartDrawTrace();
}

void AFMBBaseMeleeWeapon::StopDrawTrace()
{
    HitActors.Empty();

    GetWorldTimerManager().ClearTimer(DrawTraceTimerHandle);
    SwordTrailFXComponent->Deactivate();
}

void AFMBBaseMeleeWeapon::StartDrawTrace()
{
    SwordTrailFXComponent->Activate(true);

    SpawnSwordSlashSound();

    if (HitActors.Num() != 0)
    {
        HitActors.Empty();
        UE_LOG(LogFMBBaseMeleeWeapon, Warning, TEXT("HitActors not empty"));
    }
    GetWorldTimerManager().SetTimer(DrawTraceTimerHandle, this, &AFMBBaseMeleeWeapon::DrawTrace, 0.01f, true);
}

void AFMBBaseMeleeWeapon::DrawTrace()
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

FVector AFMBBaseMeleeWeapon::FindBladeSocketLocation(const FName BladeTraceSocketName) const
{
    const FTransform BladeSocketTransform = ItemMesh->GetSocketTransform(BladeTraceSocketName);
    const FVector BladeTrace = BladeSocketTransform.GetLocation();
    return BladeTrace;
}

void AFMBBaseMeleeWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const
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

void AFMBBaseMeleeWeapon::SortEqualCharacter(const FHitResult& HitResult)
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

void AFMBBaseMeleeWeapon::NewDamagedActor(const FHitResult& HitResult)
{
    HitActors.AddUnique(HitResult.GetActor());
    MakeDamage(HitResult);
    // UE_LOG(BaseWeaponLog, Display, TEXT("Hit %s"), *(Cast<ACharacter>(HitResult.GetActor()))->GetName());
    // DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 20.0f, 32, FColor::Green, false, 5.0f);
}

void AFMBBaseMeleeWeapon::MakeDamage(const FHitResult& HitResult)
{
    const auto DamagedActor = HitResult.GetActor();
    if (!DamagedActor) return;

    DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetController(), this);
}

void AFMBBaseMeleeWeapon::SpawnSwordSlashSound() const
{
    UGameplayStatics::SpawnSoundAttached(SwordSlashSound, //
        ItemMesh,                                         //
        SwordTrailSocketName,                             //
        FVector::ZeroVector,                              //
        FRotator::ZeroRotator,                            //
        EAttachLocation::SnapToTarget,                    //
        true);
}
