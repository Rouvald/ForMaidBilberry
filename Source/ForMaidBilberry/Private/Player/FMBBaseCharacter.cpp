// For Maid Bilberry Game. All Rights Recerved

#include "Player/FMBBaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Controller.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

#include "Components/FMBCharacterMovementComponent.h"
#include "Components/FMBHealthComponent.h"

DECLARE_LOG_CATEGORY_CLASS(BaseCharacterLog, All, All);

AFMBBaseCharacter::AFMBBaseCharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<UFMBCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;
    //
    GetCharacterMovement()->JumpZVelocity = 500.0f;
    //
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
    //
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 700.0f, 0.0f);
    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->bUseControllerDesiredRotation = true;
    GetCharacterMovement()->bIgnoreBaseRotation = true;
    //
    GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));

    HealthComponent = CreateDefaultSubobject<UFMBHealthComponent>(TEXT("HealthComponent"));

    Backpack = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Backpack"));
    Backpack->SetupAttachment(GetMesh(), BackpackSocketName);
    Backpack->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void AFMBBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    checkf(GetMesh(), TEXT("Mesh = nullptr"));
    checkf(HealthComponent, TEXT("HealthComponent = nullptr"));

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

bool AFMBBaseCharacter::IsRunning()
{
    return false;
}

void AFMBBaseCharacter::OnDeath()
{
    UE_LOG(BaseCharacterLog, Display, TEXT("Player %s is dead"), *GetName());

    GetCharacterMovement()->DisableMovement();
    SetLifeSpan(LifeSpanOnDeath);
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);

    UGameplayStatics::PlaySoundAtLocation(GetWorld(), CharacterDeathSound, GetActorLocation());
}

void AFMBBaseCharacter::OnGroundLanded(const FHitResult& HitResult)
{
    const auto VelocityZ = GetVelocity().Z * (-1);
    // UE_LOG(BaseCharacterLog, Display, TEXT("VelocityZ %f"), VelocityZ);

    if (VelocityZ < LandedVelocityZ.X) return;

    const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedVelocityZ, LandedDamage, VelocityZ);
    // UE_LOG(BaseCharacterLog, Display, TEXT("FinalDamage %f"), FinalDamage);
    TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}

/*
 *void AFMBBaseCharacter::SetTeamSkeletalMesh(USkeletalMesh* TeamSkeletalMesh) const
{
    if (TeamSkeletalMesh)
    {
        GetMesh()->SetSkeletalMesh(TeamSkeletalMesh, true);
    }
}
*/
