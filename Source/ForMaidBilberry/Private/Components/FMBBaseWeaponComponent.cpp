// For Maid Bilberry Game. All Rights Recerved

#include "Components/FMBBaseWeaponComponent.h"
#include "FMBBaseCharacter.h"
#include "Animation/FMBAnimFinishedNotify.h"
#include "Weapon/FMBBaseWeapon.h"
#include "Animation/FMBAnimUtils.h"
#include "Components/FMBCharacterMovementComponent.h"
#include "Components/FMBStaminaComponent.h"
#include "FMBCoreTypes.h"
#include "FMBUtils.h"
#include "Animation/FMBFinishedAnimNotifyState.h"

DECLARE_LOG_CATEGORY_CLASS(LogFMBBaseWeaponComponent, All, All);

UFMBBaseWeaponComponent::UFMBBaseWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UFMBBaseWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    // checkf(WeaponClasses.Num() > 0, TEXT("Character don't have weapon"));
    // CurrentWeaponIndex = 0;

    Character = Cast<AFMBBaseCharacter>(GetOwner());
    if (Character)
    {
        MovementComponent = Character->FindComponentByClass<UFMBCharacterMovementComponent>();
        StaminaComponent = Character->FindComponentByClass<UFMBStaminaComponent>();
        CheckWeaponAnimationsData();
    }
    InitWeaponComponent();
}

void UFMBBaseWeaponComponent::InitWeaponComponent()
{
    if (Character)
    {
        EquipItems();
    }
}

void UFMBBaseWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (CurrentWeapon)
    {
        CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        CurrentWeapon->Destroy();
        CurrentWeapon = nullptr;
    }
    /*if(!CurrentWeapon)
    UE_LOG(LogFMBBaseWeaponComponent, Display, TEXT("currentweapon == nullptr"));*/

    Super::EndPlay(EndPlayReason);
}

void UFMBBaseWeaponComponent::EquipItems()
{
    if (!GetWorld()) return;
    if (!Character) return;

    EquipWeapon(SpawnWeapon());
}

AFMBBaseWeapon* UFMBBaseWeaponComponent::SpawnWeapon() const
{
    if (!GetWorld() || !Character || !WeaponClass) return nullptr;

    const auto DefaultWeapon{GetWorld()->SpawnActor<AFMBBaseWeapon>(WeaponClass)};
    if (!DefaultWeapon)
    {
        UE_LOG(LogFMBBaseWeaponComponent, Display, TEXT("Error: spawn weapon"));
        return nullptr;
    }
    // DefaultWeapon->OnItemStateChanged.Broadcast(EItemState::EIS_PickUp);
    return DefaultWeapon;
}

void UFMBBaseWeaponComponent::EquipWeapon(AFMBBaseWeapon* EquippedWeapon)
{
    if (!EquippedWeapon || !Character) return;
    EquippedWeapon->OnItemStateChanged.Broadcast(EItemState::EIS_Equipped);
    EquippedWeapon->SetOwner(Character);

    if (WeaponsAnimationsData.Contains(EquippedWeapon->GetWeaponType()))
    {
        CurrentWeaponAnimationsData = WeaponsAnimationsData[EquippedWeapon->GetWeaponType()];
    }
    FMBUtils::AttachItemToSocket(EquippedWeapon, Character->GetMesh(), CurrentWeaponAnimationsData.WeaponEquipSocketName);
    CurrentWeapon = EquippedWeapon;
}

void UFMBBaseWeaponComponent::FastMeleeAttack()
{
    if (!CanDoAttack(EStaminaSpend::ESS_FastAttack)) return;
    SpendStamina(EStaminaSpend::ESS_FastAttack);

    bIsAttackAnimInProgress = true;
    PlayAnimMontage(CurrentWeaponAnimationsData.FastAttack);
}

void UFMBBaseWeaponComponent::StrongMeleeAttack()
{
    if (!CanDoAttack(EStaminaSpend::ESS_StrongAttack)) return;
    SpendStamina(EStaminaSpend::ESS_StrongAttack);

    bIsAttackAnimInProgress = true;
    PlayAnimMontage(CurrentWeaponAnimationsData.StrongAttack);
}

void UFMBBaseWeaponComponent::SpendStamina(const EStaminaSpend StaminaSpend) const
{
    if (!Character || !Character->IsPlayerControlled() || !StaminaComponent) return;
    StaminaComponent->SpendStamina(StaminaSpend);
}

void UFMBBaseWeaponComponent::PlayAnimMontage(UAnimMontage* Animation) const
{
    if (!Character) return;
    Character->PlayAnimMontage(Animation);
}

void UFMBBaseWeaponComponent::CheckWeaponAnimationsData()
{
    if (WeaponsAnimationsData.Contains(EWeaponType::EWT_NoWeapon))
    {
        InitAnimation(WeaponsAnimationsData[EWeaponType::EWT_NoWeapon]);
    }
    if (WeaponsAnimationsData.Contains(EWeaponType::EWT_SwordShield))
    {
        InitAnimation(WeaponsAnimationsData[EWeaponType::EWT_SwordShield]);
    }
    if (WeaponsAnimationsData.Contains(EWeaponType::EWT_TwoHandSword))
    {
        InitAnimation(WeaponsAnimationsData[EWeaponType::EWT_TwoHandSword]);
    }
}

void UFMBBaseWeaponComponent::InitAnimation(const FWeaponAnimationsData& WeaponAnimationData)
{
    if (!Character) return;

    const auto RollingEvent = FMBAnimUtils::FindNotifyByClass<UFMBAnimFinishedNotify>(WeaponAnimationData.Roll);
    if (MovementComponent && RollingEvent)
    {
        RollingEvent->OnNotify.AddUObject(MovementComponent, &UFMBCharacterMovementComponent::OnRollingFinished);
    }
    else
    {
        UE_LOG(LogFMBBaseWeaponComponent, Error, TEXT("Rolling Finished anim notify don't set"));
        checkNoEntry();
    }
    if (WeaponAnimationData.FastAttack)
    {
        CheckAttackAnimNotifyState(WeaponAnimationData.FastAttack);
    }
    if (WeaponAnimationData.StrongAttack)
    {
        CheckAttackAnimNotifyState(WeaponAnimationData.StrongAttack);
    }
}

void UFMBBaseWeaponComponent::CheckAttackAnimNotifyState(UAnimMontage* Animation)
{
    const auto AttackEventNotify = FMBAnimUtils::FindNotifyByClass<UFMBAnimFinishedNotify>(Animation);
    const auto AttackEventNotifyState = FMBAnimUtils::FindNotifyByClass<UFMBFinishedAnimNotifyState>(Animation);
    if (AttackEventNotify)
    {
        AttackEventNotify->OnNotify.AddUObject(this, &UFMBBaseWeaponComponent::OnAttackNotifyAnimEnd);
    }
    else
    {
        UE_LOG(LogFMBBaseWeaponComponent, Error, TEXT("Attack finished weapon anim notify don't set"));
        checkNoEntry();
    }
    if (AttackEventNotifyState)
    {
        AttackEventNotifyState->OnNotifyStateBegin.AddUObject(this, &UFMBBaseWeaponComponent::OnAttackNotifyStateBegin);
        AttackEventNotifyState->OnNotifyStateEnd.AddUObject(this, &UFMBBaseWeaponComponent::OnAttackNotifyStateEnd);
    }
    else
    {
        UE_LOG(LogFMBBaseWeaponComponent, Error, TEXT("Attack finished weapon anim notify state don't set"));
        checkNoEntry();
    }
}

void UFMBBaseWeaponComponent::OnAttackNotifyStateBegin(USkeletalMeshComponent* MeshComp)
{
    if (!Character || Character->GetMesh() != MeshComp) return;
    CurrentWeapon->MeleeAttack(EChooseAttack::ECA_FastAttack);
}

void UFMBBaseWeaponComponent::OnAttackNotifyStateEnd(USkeletalMeshComponent* MeshComp)
{
    if (!Character || Character->GetMesh() != MeshComp) return;
    if (StaminaComponent)
    {
        StaminaComponent->StartHealStamina();
    }
    StopDrawTrace();
}

void UFMBBaseWeaponComponent::OnAttackNotifyAnimEnd(USkeletalMeshComponent* MeshComp)
{
    if (!Character || Character->GetMesh() != MeshComp) return;
    bIsAttackAnimInProgress = false;
}

/*void UFMBBaseWeaponComponent::EquipShield()
{
    if (!CurrentShield) return;

    if (!WeaponShieldMaps.Contains(CurrentWeapon->GetWeaponType()))
    {
        /* @todo: for future.
        // CurrentShield->StopDrawTrace();
        if (CurrentShield->GetRootComponent())
        {
            CurrentShield->GetRootComponent()->SetVisibility(false, true);
        }
        FMBUtils::AttachItemToSocket(CurrentShield, Character->GetMesh(), WeaponArmorySocketName);
    }
    else
    {
        if (CurrentShield->GetRootComponent())
        {
            CurrentShield->GetRootComponent()->SetVisibility(true, true);
        }
        FMBUtils::AttachItemToSocket(CurrentShield, Character->GetMesh(), WeaponShieldMaps[CurrentWeapon->GetWeaponType()]);
    }
}*/

void UFMBBaseWeaponComponent::StopDrawTrace()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->StopDrawTrace();
    }
}

bool UFMBBaseWeaponComponent::CanDoAttack(const EStaminaSpend AttackStaminaSpend) const
{
    if (!CurrentWeapon) return false;
    if (!CanAttack()) return false;

    if (Character && Character->IsPlayerControlled())
    {
        if (!StaminaComponent || !(StaminaComponent->CanSpendStamina(AttackStaminaSpend))) return false;
    }
    return true;
}

bool UFMBBaseWeaponComponent::CanAttack() const
{
    if (bIsAttackAnimInProgress) return false;

    if (!CanEquip()) return false;

    // if (!MovementComponent /*|| MovementComponent->IsFalling()*/ || !(MovementComponent->CanRolling())) return false;

    return true;
}

/*UTexture2D* UFMBBaseWeaponComponent::GetCurrentWeaponUIImage() const
{
    if (CurrentWeapon)
    {
        return CurrentWeapon->GetItemData().ItemIcon;
    }
    return nullptr;
}*/

// work if u have only 2 weapons //
/*bool UFMBBaseWeaponComponent::GetArmoryWeaponUIData(FItemData& WeaponUIData) const
{
    if (ArmoryWeapon)
    {
        WeaponUIData = ArmoryWeapon->GetWeaponUIData();
        return true;
    }
    return false;
}*/
