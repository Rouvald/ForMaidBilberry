// For Maid Bilberry Game. All Rights Recerved


#include "Weapon/FMBBaseWeapon.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FMBBaseCharacter.h"
#include "Components/FMBWeaponComponent.h"

DECLARE_LOG_CATEGORY_CLASS(BaseWeaponLog, All, All);

AFMBBaseWeapon::AFMBBaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    DefaultRootComponent =CreateDefaultSubobject<USceneComponent>("DefaultRootComponent");
    RootComponent = DefaultRootComponent;

    WeaponMesh= CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");
    WeaponMesh->SetupAttachment(DefaultRootComponent);
    WeaponMesh->SetCollisionResponseToChannels(ECollisionResponse::ECR_Overlap);
    
    //WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    //SetRootComponent(WeaponMesh);
}

void AFMBBaseWeapon::BeginPlay()
{
    Super::BeginPlay();
    check(WeaponMesh);
}

void AFMBBaseWeapon::FastMeleeAttack()
{
    //UE_LOG(BaseWeaponLog, Display, TEXT("Fast Melee Attack"));
    if (!GetWorld()) return;

    DamageAmount = FastAttackDamage;

    StartDrawTrace();
}

void AFMBBaseWeapon::StrongMeleeAttack()
{
    //UE_LOG(BaseWeaponLog, Display, TEXT("Strong Melee Attack"));
    if (!GetWorld()) return;

    DamageAmount= StrongAttackDamage;

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

    DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 0.05f, 0, 5.0f);

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
    if(!GetWorld()) return;
    
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());

    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

void AFMBBaseWeapon::SortEqualCharacter(FHitResult& HitResult)
{
    if (HitActors.Num() == 0)
    {
        NewDamagedActor(HitResult);
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
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 32, FColor::Green, false, 5.0f);
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
    GetWorld()->GetTimerManager().SetTimer(DrawTraceTimerHandle, this, &AFMBBaseWeapon::TraceLogic, 0.01f, true);
}

void AFMBBaseWeapon::StopDrawTrace()
{
    //UE_LOG(BaseWeaponLog, Display, TEXT("Num of HitActors: %i"), HitActors.Num());
    //UE_LOG(BaseWeaponLog, Display, TEXT("Get Owner: %s"), *(GetOwner()->GetName()));
    HitActors.Empty();

    if(GetWorld()->GetTimerManager().IsTimerActive(DrawTraceTimerHandle))
    {
        GetWorld()->GetTimerManager().ClearTimer(DrawTraceTimerHandle);
    }
}
