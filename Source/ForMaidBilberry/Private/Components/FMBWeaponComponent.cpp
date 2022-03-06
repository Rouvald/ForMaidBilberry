// For Maid Bilberry Game. All Rights Recerved

#include "Components/FMBWeaponComponent.h"
#include "FMBBaseCharacter.h"
#include "FMBBaseShield.h"
#include "Animation/FMBAnimFinishedNotify.h"
#include "GameFramework/Character.h"
#include "Weapon/FMBBaseWeapon.h"
#include "Animation/FMBAnimUtils.h"
#include "Animation/FMBChangeEquipWeaponAnimNotify.h"
#include "Components/FMBCharacterMovementComponent.h"
#include "Components/FMBStaminaComponent.h"
#include "FMBCoreTypes.h"
#include "FMBUtils.h"
#include "Animation/FMBFinishedAnimNotifyState.h"

DECLARE_LOG_CATEGORY_CLASS(LogFMBWeaponComponent, All, All);

UFMBWeaponComponent::UFMBWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UFMBWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    checkf(WeaponClasses.Num() > 0, TEXT("Character don't have weapon"));
    CurrentWeaponIndex = 0;

    Character = Cast<AFMBBaseCharacter>(GetOwner());
    if (Character)
    {
        MovementComponent = Character->FindComponentByClass<UFMBCharacterMovementComponent>();
        StaminaComponent = Character->FindComponentByClass<UFMBStaminaComponent>();
        CheckWeaponAnimationsData();
        SpawnItems();
        EquipWeapon();
    }
}

void UFMBWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    CurrentShield->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    CurrentShield->Destroy();

    CurrentWeapon = nullptr;
    for (const auto Weapon : Weapons)
    {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->Destroy();
    }
    Weapons.Empty();

    Super::EndPlay(EndPlayReason);
}

void UFMBWeaponComponent::SpawnItems()
{
    if (!GetWorld()) return;
    if (!Character) return;

    SpawnWeapons();
    SpawnShields();
}

void UFMBWeaponComponent::SpawnWeapons()
{
    for (auto WeaponClass : WeaponClasses)
    {
        auto Weapon = GetWorld()->SpawnActor<AFMBBaseWeapon>(WeaponClass);
        if (!Weapon) continue;

        Weapon->SetOwner(Character);

        Weapons.Add(Weapon);

        if (Weapon->GetRootComponent())
        {
            Weapon->GetRootComponent()->SetVisibility(false, true);
        }
        FMBUtils::AttachItemToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
    }
}

void UFMBWeaponComponent::SpawnShields()
{
    CurrentShield = GetWorld()->SpawnActor<AFMBBaseShield>(ShieldClass);
    if (!CurrentShield) return;

    CurrentShield->SetOwner(Character);

    if (CurrentShield->GetRootComponent())
    {
        CurrentShield->GetRootComponent()->SetVisibility(false, true);
    }
    FMBUtils::AttachItemToSocket(CurrentShield, Character->GetMesh(), WeaponArmorySocketName);
}

void UFMBWeaponComponent::EquipWeapon()
{
    if (!Character) return;

    EquipAnimInProgress = true;
    if (CurrentWeapon)
    {
        PlayAnimMontage(CurrentWeaponAnimationsData.Equip);
    }
    else
    {
        PlayAnimMontage(WeaponsAnimationsData[EWeaponType::EWT_RedSword].Equip);
    }
    //==================================================
    // UE_LOG(BaseWeaponComponentLog, Display, TEXT("Equip"));
    //==================================================
}

void UFMBWeaponComponent::NextWeapon()
{
    if (!CanEquip() || !CanAttack()) return;
    int32 CurrentWeaponIndexTemp = CurrentWeaponIndex;
    ++CurrentWeaponIndexTemp;
    CurrentWeaponIndex = (CurrentWeaponIndexTemp) % Weapons.Num();
    EquipWeapon();
}

void UFMBWeaponComponent::FastMeleeAttack()
{
    if (!CanDoAttack(EStaminaSpend::ESS_FastAttack)) return;
    SpendStamina(EStaminaSpend::ESS_FastAttack);

    AttackAnimInProgress = true;
    PlayAnimMontage(CurrentWeaponAnimationsData.FastAttack);
}

void UFMBWeaponComponent::StrongMeleeAttack()
{
    if (!CanDoAttack(EStaminaSpend::ESS_StrongAttack)) return;
    SpendStamina(EStaminaSpend::ESS_StrongAttack);

    AttackAnimInProgress = true;
    PlayAnimMontage(CurrentWeaponAnimationsData.StrongAttack);
}

void UFMBWeaponComponent::SpendStamina(const EStaminaSpend StaminaSpend) const
{
    if (!Character || !Character->IsPlayerControlled() || !StaminaComponent) return;
    StaminaComponent->SpendStamina(StaminaSpend);
}

void UFMBWeaponComponent::PlayAnimMontage(UAnimMontage* Animation) const
{
    if (!Character) return;
    Character->PlayAnimMontage(Animation);
}

void UFMBWeaponComponent::CheckWeaponAnimationsData()
{
    if (WeaponsAnimationsData.Contains(EWeaponType::EWT_RedSword))
    {
        InitAnimation(WeaponsAnimationsData[EWeaponType::EWT_RedSword]);
    }
    if (WeaponsAnimationsData.Contains(EWeaponType::EWT_YellowSword))
    {
        InitAnimation(WeaponsAnimationsData[EWeaponType::EWT_YellowSword]);
    }
}

void UFMBWeaponComponent::InitAnimation(const FWeaponAnimationsData& WeaponAnimationData)
{
    if (!Character) return;

    const auto RollingEvent = FMBAnimUtils::FindNotifyByClass<UFMBAnimFinishedNotify>(WeaponAnimationData.Roll);
    if (MovementComponent && RollingEvent)
    {
        RollingEvent->OnNotify.AddUObject(MovementComponent, &UFMBCharacterMovementComponent::OnRollingFinished);
    }
    else
    {
        UE_LOG(LogFMBWeaponComponent, Error, TEXT("Rolling Finished anim notify don't set"));
        checkNoEntry();
    }
    const auto ChangeEquipWeapon = FMBAnimUtils::FindNotifyByClass<UFMBChangeEquipWeaponAnimNotify>(WeaponAnimationData.Equip);
    if (ChangeEquipWeapon)
    {
        ChangeEquipWeapon->OnNotify.AddUObject(this, &UFMBWeaponComponent::OnChangeEquipWeapon);
    }
    else
    {
        UE_LOG(LogFMBWeaponComponent, Error, TEXT("Change weapon anim notify don't set"));
        checkNoEntry();
    }
    const auto EquipFinished = FMBAnimUtils::FindNotifyByClass<UFMBAnimFinishedNotify>(WeaponAnimationData.Equip);
    if (EquipFinished)
    {
        EquipFinished->OnNotify.AddUObject(this, &UFMBWeaponComponent::OnEquipFinished);
    }
    else
    {
        UE_LOG(LogFMBWeaponComponent, Error, TEXT("Equip weapon anim notify don't set"));
        checkNoEntry();
    }

    CheckAttackAnimNotifyState(WeaponAnimationData.FastAttack);
    CheckAttackAnimNotifyState(WeaponAnimationData.StrongAttack);
}

void UFMBWeaponComponent::CheckAttackAnimNotifyState(UAnimMontage* Animation)
{
    const auto AttackEvent = FMBAnimUtils::FindNotifyByClass<UFMBFinishedAnimNotifyState>(Animation);
    const auto AttackAnimEvent = FMBAnimUtils::FindNotifyByClass<UFMBAnimFinishedNotify>(Animation);
    if (AttackEvent && AttackAnimEvent)
    {
        AttackEvent->OnNotifyStateBegin.AddUObject(this, &UFMBWeaponComponent::OnAttackNotifyStateBegin);
        AttackEvent->OnNotifyStateEnd.AddUObject(this, &UFMBWeaponComponent::OnAttackNotifyStateEnd);

        AttackAnimEvent->OnNotify.AddUObject(this, &UFMBWeaponComponent::OnAttackNotifyAnimEnd);
    }
    else
    {
        UE_LOG(LogFMBWeaponComponent, Error, TEXT("Attack finished weapon anim notifies don't set"));
        checkNoEntry();
    }
}

void UFMBWeaponComponent::OnAttackNotifyStateBegin(USkeletalMeshComponent* MeshComp)
{
    if (!Character || Character->GetMesh() != MeshComp) return;
    CurrentWeapon->MeleeAttack(EChooseAttack::ECA_FastAttack);
}

void UFMBWeaponComponent::OnAttackNotifyStateEnd(USkeletalMeshComponent* MeshComp)
{
    if (!Character || Character->GetMesh() != MeshComp) return;
    if (StaminaComponent)
    {
        StaminaComponent->StartHealStamina();
    }
    StopDrawTrace();
}

void UFMBWeaponComponent::OnAttackNotifyAnimEnd(USkeletalMeshComponent* MeshComp)
{
    if (!Character || Character->GetMesh() != MeshComp) return;
    AttackAnimInProgress = false;
}

void UFMBWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp)
{
    if (!Character || Character->GetMesh() != MeshComp) return;
    EquipAnimInProgress = false;
}

void UFMBWeaponComponent::OnChangeEquipWeapon(USkeletalMeshComponent* MeshComp)
{
    if (CurrentWeaponIndex < 0 || CurrentWeaponIndex > Weapons.Num())
    {
        UE_LOG(LogFMBWeaponComponent, Display, TEXT("Incorrect Weapon Index"));
        return;
    }
    if (!Character || Character->GetMesh() != MeshComp) return;

    if (CurrentWeapon)
    {
        CurrentWeapon->StopDrawTrace();
        if (CurrentWeapon->GetRootComponent())
        {
            CurrentWeapon->GetRootComponent()->SetVisibility(false, true);
        }
        FMBUtils::AttachItemToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
    }
    CurrentWeapon = Weapons[CurrentWeaponIndex];
    EquipShield();
    // only for 2 weapons
    ArmoryWeapon = Weapons[(CurrentWeaponIndex + 1) % Weapons.Num()];
    if (CurrentWeapon->GetRootComponent())
    {
        CurrentWeapon->GetRootComponent()->SetVisibility(true, true);
    }
    if (WeaponsAnimationsData.Contains(CurrentWeapon->GetWeaponType()))
    {
        CurrentWeaponAnimationsData = WeaponsAnimationsData[CurrentWeapon->GetWeaponType()];
    }
    FMBUtils::AttachItemToSocket(CurrentWeapon, Character->GetMesh(), CurrentWeaponAnimationsData.WeaponEquipSocketName);
}

void UFMBWeaponComponent::EquipShield()
{
    if (!CurrentShield) return;

    if (!WeaponShieldMaps.Contains(CurrentWeapon->GetWeaponType()))
    {
        /* @todo: for future. */
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
}

void UFMBWeaponComponent::OnStartBlock()
{
    if (!CurrentShield) return;

    // CurrentShield->Startblocking();
    bIsBlocking = true;
}

void UFMBWeaponComponent::OnStopBlock()
{
    // CurrentShield->StopBlocking();
    bIsBlocking = false;
}

void UFMBWeaponComponent::StopDrawTrace()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->StopDrawTrace();
    }
}

bool UFMBWeaponComponent::CanDoAttack(const EStaminaSpend AttackStaminaSpend) const
{
    if (!CurrentWeapon) return false;
    if (!CanAttack()) return false;

    if (Character && Character->IsPlayerControlled())
    {
        if (!StaminaComponent || !(StaminaComponent->CanSpendStamina(AttackStaminaSpend))) return false;
    }

    return true;
}

bool UFMBWeaponComponent::CanAttack() const
{
    if (AttackAnimInProgress) return false;

    if (!CanEquip()) return false;

    if (!MovementComponent /*|| MovementComponent->IsFalling()*/ || !(MovementComponent->CanRolling())) return false;

    return true;
}

bool UFMBWeaponComponent::CanEquip() const
{
    return !EquipAnimInProgress;
}

bool UFMBWeaponComponent::GetCurrentWeaponUIData(FWeaponUIData& WeaponUIData) const
{
    if (CurrentWeapon)
    {
        WeaponUIData = CurrentWeapon->GetWeaponUIData();
        return true;
    }
    return false;
}

// work if u have only 2 weapons //
bool UFMBWeaponComponent::GetArmoryWeaponUIData(FWeaponUIData& WeaponUIData) const
{
    if (ArmoryWeapon)
    {
        WeaponUIData = ArmoryWeapon->GetWeaponUIData();
        return true;
    }
    return false;
}
