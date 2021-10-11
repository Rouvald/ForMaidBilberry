// For Maid Bilberry Game. All Rights Recerved


#include "Components/FMBWeaponComponent.h"
#include "FMBBaseCharacter.h"
#include "GameFramework/Character.h"
#include "Weapon/FMBBaseWeapon.h"
#include "Animation/FMBAttackFinishedNotify.h"
#include "Components/FMBCharacterMovementComponent.h"

DECLARE_LOG_CATEGORY_CLASS(BaseWeaponComponentLog, All, All);

UFMBWeaponComponent::UFMBWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UFMBWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    InitAnimation(FastMeleeAttackAnimMontage);
    InitAnimation(StrongMeleeAttackAnimMontage);
    SpawnWeapon();
    
    //OnStaminaChange.AddDynamic(this, &UFMBWeaponComponent::DecreaseStaminaAttack);
}

/*
void UFMBWeaponComponent::DecreaseStaminaAttack(float DecreaseStamina)
{
    UE_LOG(BaseWeaponComponentLog, Display, TEXT("DecreaseStamina: %0.0f"), DecreaseStamina);
}
*/

void UFMBWeaponComponent::SpawnWeapon()
{
    if (!GetWorld()) return;

    const auto Character = GetCharacter();

    CurrentWeapon = GetWorld()->SpawnActor<AFMBBaseWeapon>(WeaponClass);
    if (!CurrentWeapon) return;
    
    CurrentWeapon->SetOwner(Character);
    
    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponSocketName);

}

void UFMBWeaponComponent::AttachWeaponToSocket(AFMBBaseWeapon* Weapon, USkeletalMeshComponent* MeshComp, const FName& WeaponSocket)
{
    if(!Weapon || !MeshComp) return;
    
    const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    CurrentWeapon->AttachToComponent(MeshComp, AttachmentRules, WeaponSocket);
}

void UFMBWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    CurrentWeapon->Destroy();

    Super::EndPlay(EndPlayReason);
}

void UFMBWeaponComponent::FastMeleeAttack()
{
    if (!CanAttack()) return;
    if (!CurrentWeapon) return;

    CurrentWeapon->FastMeleeAttack();

    AttackAnimInProgress = true;
    StopMovement();
    PlayAnimMontage(FastMeleeAttackAnimMontage);
}

void UFMBWeaponComponent::StrongMeleeAttack()
{
    if (!CanAttack()) return;
    if (!CurrentWeapon) return;


    CurrentWeapon->StrongMeleeAttack();
    
    AttackAnimInProgress = true;
    StopMovement();
    PlayAnimMontage(StrongMeleeAttackAnimMontage);
}

void UFMBWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
    const auto Character = GetCharacter();
    if (!Character) return;

    Character->PlayAnimMontage(Animation);
}

void UFMBWeaponComponent::InitAnimation(UAnimMontage* Animation)
{
    if (!Animation) return;

    const auto NotifyEvents = Animation->Notifies;
    for (const auto NotifyEvent : NotifyEvents)
    {
        const auto MeleeAttackEvent = Cast<UFMBAttackFinishedNotify>(NotifyEvent.Notify);
        if (MeleeAttackEvent)
        {
            MeleeAttackEvent->OnNotified.AddUObject(this, &UFMBWeaponComponent::OnAttackFinished);
            break;
        }
    }
}

void UFMBWeaponComponent::OnAttackFinished(USkeletalMeshComponent* MeshComp)
{
    const auto Character = GetCharacter();
    if (!Character || Character->GetMesh() != MeshComp) return;

    StopDrawTrace();

    AttackAnimInProgress = false;
    StartMovement();
    //UE_LOG(BaseWeaponComponentLog, Display, TEXT("Attack Finished"));
}

void UFMBWeaponComponent::StopDrawTrace() const
{
    CurrentWeapon->StopDrawTrace();
}

bool UFMBWeaponComponent::CanAttack() const
{
    const auto Character = Cast<AFMBBaseCharacter>(GetOwner());
    if (!Character) return AttackAnimInProgress;

    if (Character->GetVelocity().Z != 0.0f) return AttackAnimInProgress;

    return !AttackAnimInProgress;
}

void UFMBWeaponComponent::StartMovement()
{
    const auto MovementComponent = GetMovementComponent();

    if (!AttackAnimInProgress)
    {
        MovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);
    }
}

void UFMBWeaponComponent::StopMovement()
{
    const auto MovementComponent = GetMovementComponent();

    if (AttackAnimInProgress)
    {
        MovementComponent->SetMovementMode(EMovementMode::MOVE_None);
    }
}

AFMBBaseCharacter* UFMBWeaponComponent::GetCharacter()
{
    const auto Character = Cast<AFMBBaseCharacter>(GetOwner());
    if (!Character) return nullptr;

    return Character;
}

UCharacterMovementComponent* UFMBWeaponComponent::GetMovementComponent()
{
    const auto Character = GetCharacter();

    const auto MovementComponent = Cast<UCharacterMovementComponent>(Character->GetMovementComponent());
    if (!MovementComponent) return nullptr;

    return MovementComponent;
}
