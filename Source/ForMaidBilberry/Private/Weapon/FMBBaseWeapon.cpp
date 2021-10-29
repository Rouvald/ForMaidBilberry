// For Maid Bilberry Game. All Rights Recerved
// For Maid Bilberry Game. All Rights Recerved


#include "Weapon/FMBBaseWeapon.h"
#include "DrawDebugHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FMBBaseCharacter.h"
#include "Weapon/Components/FMBWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

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
}

void AFMBBaseWeapon::BeginPlay()
{
    Super::BeginPlay();
    check(WeaponMesh);
    check(WeaponFXComponent);
}

void AFMBBaseWeapon::FastMeleeAttack()
{
    if (!GetWorld()) return;

    DamageAmount = FastAttackDamage;

    StartDrawTrace();
}

void AFMBBaseWeapon::StrongMeleeAttack()
{
    if (!GetWorld()) return;

    DamageAmount = StrongAttackDamage;

    StartDrawTrace();
}

APlayerController* AFMBBaseWeapon::GetPlayerController() const
{
    const auto Player = Cast<ACharacter>(GetOwner());
    if (!Player) return nullptr;

    return Player->GetController<APlayerController>();
}

void AFMBBaseWeapon::TraceLogic()
{
    if (!GetWorld()) return;

    DrawTrace();
}

void AFMBBaseWeapon::DrawTrace()
{
    const FVector TraceStart = FindBladeSocketLocation(StartBladeTraceSocketName);
    const FVector TraceEnd = FindBladeSocketLocation(EndBladeTraceSocketName);
    if (TraceStart.IsZero() || TraceEnd.IsZero()) return;

    //DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 0.05f, 0, 5.0f);

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    if (HitResult.bBlockingHit && Cast<ACharacter>(HitResult.GetActor()))
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

void AFMBBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
    if (!GetWorld()) return;

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());

    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
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
            if (HitActor == Cast<ACharacter>(HitResult.GetActor()))
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
    HitActors.Add(Cast<ACharacter>(HitResult.GetActor()));
    if (HitResult.bBlockingHit)
    {
        MakeDamage(HitResult);
        //UE_LOG(BaseWeaponLog, Display, TEXT("Hit %s"), *(Cast<ACharacter>(HitResult.GetActor()))->GetName());
        //DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 32, FColor::Green, false, 5.0f);
    }
}

void AFMBBaseWeapon::MakeDamage(FHitResult& HitResult)
{
    const auto DamagedActor = HitResult.GetActor();
    if (!DamagedActor) return;

    DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
}

void AFMBBaseWeapon::StartDrawTrace()
{
    InitSwordTrailFX();
    /*
     * maybe its a little mistake, in future i will test it ;)
     * This line need if u wanna use FMBSwordTrailAnimNotifyState
     * U need comment InitSwordTrailFX(); , and uncomment SwordTrailFXComponent = SpawnSwordTrailFX();
    */
    //SwordTrailFXComponent = SpawnSwordTrailFX();
    GetWorld()->GetTimerManager().SetTimer(DrawTraceTimerHandle, this, &AFMBBaseWeapon::TraceLogic, 0.01f, true);
}

void AFMBBaseWeapon::StopDrawTrace()
{
    //UE_LOG(BaseWeaponLog, Display, TEXT("Num of HitActors: %i"), HitActors.Num());
    //UE_LOG(BaseWeaponLog, Display, TEXT("Get Owner: %s"), *(GetOwner()->GetName()));
    HitActors.Empty();

    if (GetWorld()->GetTimerManager().IsTimerActive(DrawTraceTimerHandle))
    {
        GetWorld()->GetTimerManager().ClearTimer(DrawTraceTimerHandle);
    }
    /*
    * This line need if u wanna use FMBSwordTrailAnimNotifyState
    * U need comment SetSwordTrailFXVisibility(false);
    */
    SetSwordTrailFXVisibility(false);
}

UNiagaraComponent* AFMBBaseWeapon::SpawnSwordTrailFX() const
{
    return UNiagaraFunctionLibrary::SpawnSystemAttached
        (
            SwordTrailFX,                  //
            WeaponMesh,                    //
            SwordTrailSocketName,          //
            FVector::ZeroVector,           //
            FRotator::ZeroRotator,         //
            EAttachLocation::SnapToTarget, //
            true);
}

void AFMBBaseWeapon::InitSwordTrailFX()
{
    if (!SwordTrailFXComponent)
    {
        SwordTrailFXComponent = SpawnSwordTrailFX();
    }
    SetSwordTrailFXVisibility(true);
}

void AFMBBaseWeapon::SetSwordTrailFXVisibility(bool Visible) const
{
    if (SwordTrailFXComponent)
    {
        //SwordTrailFXComponent->SetPaused(!Visible);
        SwordTrailFXComponent->SetVisibility(Visible);
    }
}
