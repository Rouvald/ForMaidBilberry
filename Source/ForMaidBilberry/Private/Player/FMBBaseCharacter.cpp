// For Maid Bilberry Game. All Rights Recerved

#include "Player/FMBBaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/FMBHealthComponent.h"
#include "Components/FMBWeaponComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Controller.h"
#include "Weapon/FMBBaseWeapon.h"
#include "Components/CapsuleComponent.h"

DECLARE_LOG_CATEGORY_CLASS(BaseCharacterLog, All, All);

AFMBBaseCharacter::AFMBBaseCharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<UFMBCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;
    //
    GetCharacterMovement()->JumpZVelocity = 550.0f;
    //
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
    //
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 700.0f, 0.0f);
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->bUseControllerDesiredRotation = true;
    GetCharacterMovement()->bIgnoreBaseRotation = true;
    //
    GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -86.0f), FRotator(0.0f, -90.0f, 0.0f));

    HealthComponent = CreateDefaultSubobject<UFMBHealthComponent>("HealthComponent");
    WeaponComponent = CreateDefaultSubobject<UFMBWeaponComponent>("WeaponComponent");

    Backpack = CreateDefaultSubobject<UStaticMeshComponent>("Backpack");
    Backpack->SetupAttachment(GetMesh(), BackpackSocketName);
    Backpack->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void AFMBBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(GetMesh());
    check(HealthComponent);
    check(GetCharacterMovement());

    OnHealthChange(HealthComponent->GetHealth(), 0.0f);
    HealthComponent->OnDeath.AddUObject(this, &AFMBBaseCharacter::OnDeath);
    HealthComponent->OnHealthChange.AddUObject(this, &AFMBBaseCharacter::OnHealthChange);

    LandedDelegate.AddDynamic(this, &AFMBBaseCharacter::OnGroundLanded);
}

void AFMBBaseCharacter::OnHealthChange(float Health, float HealthDelta) {}

void AFMBBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

bool AFMBBaseCharacter::IsRunning() const
{
    return false;
}

/*
float AFMBBaseCharacter::GetMovementDirection() const
{
    if (GetVelocity().IsZero()) return 0.0f;
    const auto VelosityNormal = GetVelocity().GetSafeNormal();
    const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelosityNormal));
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelosityNormal);
    const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
    return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}
*/

void AFMBBaseCharacter::OnDeath()
{
    UE_LOG(BaseCharacterLog, Display, TEXT("Player %s is dead"), *GetName());

    // PlayAnimMontage(DeathAnimMontage);

    GetCharacterMovement()->DisableMovement();
    SetLifeSpan(LifeSpanOnDeath);
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

    WeaponComponent->StopDrawTrace();

    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);
}

void AFMBBaseCharacter::OnGroundLanded(const FHitResult& Hitresult)
{
    const auto VelocityZ = GetVelocity().Z * (-1);
    // UE_LOG(BaseCharacterLog, Display, TEXT("VelocityZ %f"), VelocityZ);

    if (VelocityZ < LandedVelocityZ.X) return;

    const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedVelocityZ, LandedDamage, VelocityZ);
    UE_LOG(BaseCharacterLog, Display, TEXT("FinalDamage %f"), FinalDamage);
    TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}

void AFMBBaseCharacter::SetTeamSkeletalMesh(USkeletalMesh* TeamSkeletalMesh) const
{
    if (TeamSkeletalMesh)
    {
        GetMesh()->SetSkeletalMesh(TeamSkeletalMesh, true);
    }
}
