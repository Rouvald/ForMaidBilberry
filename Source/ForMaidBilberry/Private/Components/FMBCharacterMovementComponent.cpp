// For Maid Bilberry Game. All Rights Reserved

#include "Components/FMBCharacterMovementComponent.h"
#include "Components/FMBStaminaComponent.h"
#include "Components//FMBBaseWeaponComponent.h"
#include "Player/FMBBaseCharacter.h"
#include "FMBUtils.h"
#include "Components/CapsuleComponent.h"

DECLARE_LOG_CATEGORY_CLASS(LogFMBCharacterMovementComponent, All, All);

void UFMBCharacterMovementComponent::BeginPlay()
{
    Super::BeginPlay();

    if (GetOwner())
    {
        Character = Cast<AFMBBaseCharacter>(GetOwner());
    }
}

float UFMBCharacterMovementComponent::GetMaxSpeed() const
{
    const auto MaxSpeed = Super::GetMaxSpeed();
    return Character && Character->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed;
}

void UFMBCharacterMovementComponent::Rolling()
{
    if (IsFalling() || !CanRolling()) return;

    if (!Character) return;

    const auto WeaponComponent = FMBUtils::GetFMBPlayerComponent<UFMBBaseWeaponComponent>(Character);
    if (!WeaponComponent || !(WeaponComponent->CanEquip()) || !(WeaponComponent->CanAttack())) return;

    const auto StaminaComponent = FMBUtils::GetFMBPlayerComponent<UFMBStaminaComponent>(Character);
    if (!StaminaComponent || !(StaminaComponent->CanSpendStamina(EStaminaSpend::ESS_Rolling))) return;

    if (!Velocity.IsZero())
    {
        const auto VelocityNormal = Velocity.GetSafeNormal();
        const auto AngleBetween = FMath::Acos(FVector::DotProduct(Character->GetActorForwardVector(), VelocityNormal));
        if (!FMath::IsNearlyZero(AngleBetween))
        {
            Character->SetActorRotation(VelocityNormal.Rotation());
        }
    }
    else
    {
        /*FVector ViewLocation;
        FRotator ViewRotation;
        if (FMBUtils::GetTraceData(Character, ViewLocation, ViewRotation))
        {
            Character->SetActorRotation(FRotator{0.0f, ViewRotation.Yaw, 0.0f});
        }*/
    }
    StaminaComponent->SpendStamina(EStaminaSpend::ESS_Rolling);
    Character->GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);

    if (WeaponComponent->GetCurrentWeapon())
    {
        RollingAnimInProgress = true;
        WeaponComponent->StopDrawTrace();
        Character->PlayAnimMontage(WeaponComponent->GetCurrentWeaponAnimationsData().Roll);
    }
    //==================================================
    // UE_LOG(MovementComponentLog, Display, TEXT("ESS_Rolling animation play"));
    //==================================================
}

void UFMBCharacterMovementComponent::OnRollingFinished(USkeletalMeshComponent* MeshComp)
{
    if (!Character || Character->GetMesh() != MeshComp) return;

    const auto StaminaComponent = FMBUtils::GetFMBPlayerComponent<UFMBStaminaComponent>(Character);
    if (!StaminaComponent) return;

    StaminaComponent->StartHealStamina();

    Character->GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
    RollingAnimInProgress = false;
}

bool UFMBCharacterMovementComponent::CanRolling() const
{
    return !RollingAnimInProgress;
}