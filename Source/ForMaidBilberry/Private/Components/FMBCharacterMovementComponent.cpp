// For Maid Bilberry Game. All Rights Recerved

#include "Components/FMBCharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/FMBHealthComponent.h"
#include "Components//FMBWeaponComponent.h"
#include "Animation//FMBAnimUtils.h"
#include "Animation/FMBAnimFinishedNotify.h"
#include "Player/FMBBaseCharacter.h"
#include "FMBUtils.h"

DECLARE_LOG_CATEGORY_CLASS(MovementComponentLog, All, All);

void UFMBCharacterMovementComponent::BeginPlay()
{
    Super::BeginPlay();
}

float UFMBCharacterMovementComponent::GetMaxSpeed() const
{
    const auto MaxSpeed = Super::GetMaxSpeed();
    const auto Character = Cast<AFMBBaseCharacter>(GetPawnOwner());

    return Character && Character->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed;
}

void UFMBCharacterMovementComponent::Rolling()
{
    const auto Character = Cast<AFMBBaseCharacter>(GetPawnOwner());
    if (!Character) return;

    RollingAnimInProgress = true;
    const auto WeaponComponent = FMBUtils::GetFMBPlayerComponent<UFMBWeaponComponent>(Character);
    if (WeaponComponent || WeaponComponent->GetCurrentWeapon())
    {
        Character->PlayAnimMontage(WeaponComponent->GetCurrentWeaponAnimationsData().Roll);
    }
    //==================================================
    //UE_LOG(MovementComponentLog, Display, TEXT("Rolling animation play"));
    //==================================================
}

void UFMBCharacterMovementComponent::OnRollingFinished(USkeletalMeshComponent* MeshComp)
{
    const auto Character = Cast<AFMBBaseCharacter>(GetPawnOwner());
    if (!Character || Character->GetMesh() != MeshComp) return;

    const auto HealthComponent = FMBUtils::GetFMBPlayerComponent<UFMBHealthComponent>(GetPawnOwner());
    if (!HealthComponent) return;

    HealthComponent->StartHealStaminaTimer();

    RollingAnimInProgress = false;
}

bool UFMBCharacterMovementComponent::CanRolling() const
{
    return !RollingAnimInProgress;
}
