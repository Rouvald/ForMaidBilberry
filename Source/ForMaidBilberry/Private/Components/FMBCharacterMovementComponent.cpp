// For Maid Bilberry Game. All Rights Recerved

#include "Components/FMBCharacterMovementComponent.h"
#include "Components/FMBStaminaComponent.h"
#include "Components//FMBWeaponComponent.h"
#include "Player/FMBBaseCharacter.h"
#include "FMBUtils.h"
#include "Components/CapsuleComponent.h"

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
    if (IsFalling() || !CanRolling()) return;

    const auto Character = Cast<AFMBBaseCharacter>(GetPawnOwner());
    if (!Character) return;

    const auto WeaponComponent = FMBUtils::GetFMBPlayerComponent<UFMBWeaponComponent>(Character);
    if (!WeaponComponent || !(WeaponComponent->CanEquip()) || !(WeaponComponent->CanAttack())) return;

    const auto StaminaComponent = FMBUtils::GetFMBPlayerComponent<UFMBStaminaComponent>(Character);
    if (!StaminaComponent || !(StaminaComponent->SpendStamina(EStaminaSpend::Rolling))) return;

    if (!Velocity.IsZero())
    {
        const auto VelocityNormal = Velocity.GetSafeNormal();
        const auto AngleBetween = FMath::Acos(FVector::DotProduct(Character->GetActorForwardVector(), VelocityNormal));
        if (!FMath::IsNearlyZero(AngleBetween))
        {
            Character->SetActorRotation(VelocityNormal.Rotation());
        }
    }

    RollingAnimInProgress = true;
    Character->GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);

    if (WeaponComponent->GetCurrentWeapon())
    {
        WeaponComponent->StopDrawTrace();
        Character->PlayAnimMontage(WeaponComponent->GetCurrentWeaponAnimationsData().Roll);
    }
    //==================================================
    // UE_LOG(MovementComponentLog, Display, TEXT("Rolling animation play"));
    //==================================================
}

void UFMBCharacterMovementComponent::OnRollingFinished(USkeletalMeshComponent* MeshComp)
{
    const auto Character = Cast<AFMBBaseCharacter>(GetPawnOwner());
    if (!Character || Character->GetMesh() != MeshComp) return;

    const auto StaminaComponent = FMBUtils::GetFMBPlayerComponent<UFMBStaminaComponent>(GetPawnOwner());
    if (!StaminaComponent) return;

    StaminaComponent->StartHealStamina();

    Character->GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
    RollingAnimInProgress = false;
}

bool UFMBCharacterMovementComponent::CanRolling() const
{
    return !RollingAnimInProgress;
}
