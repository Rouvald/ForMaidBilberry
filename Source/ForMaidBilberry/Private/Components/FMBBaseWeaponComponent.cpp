// For Maid Bilberry Game. All Rights Recerved

#include "Components/FMBBaseWeaponComponent.h"
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
        SpawnItems();
        // EquipWeapon();
    }
}

void UFMBBaseWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    CurrentWeapon->Destroy();

    /*CurrentWeapon = nullptr;
    for (const auto Weapon : Weapons)
    {

    }
    Weapons.Empty();*/

    Super::EndPlay(EndPlayReason);
}

void UFMBBaseWeaponComponent::SpawnItems()
{
    if (!GetWorld()) return;
    if (!Character) return;

    SpawnWeapon();
}

void UFMBBaseWeaponComponent::SpawnWeapon()
{
    CurrentWeapon = GetWorld()->SpawnActor<AFMBBaseWeapon>(WeaponClass);
    if (!CurrentWeapon)
    {
        UE_LOG(LogFMBWeaponComponent, Display, TEXT("Error: spawn weapon"));
        return;
    }

    CurrentWeapon->SetOwner(Character);

    if (CurrentWeapon->GetRootComponent())
    {
        CurrentWeapon->GetRootComponent()->SetVisibility(true, true);
    }
    if (WeaponsAnimationsData.Contains(CurrentWeapon->GetWeaponType()))
    {
        CurrentWeaponAnimationsData = WeaponsAnimationsData[CurrentWeapon->GetWeaponType()];
    }
    UE_LOG(LogFMBWeaponComponent, Display, TEXT("%d"), CurrentWeapon->GetWeaponType() == EWeaponType::EWT_YellowSword);
    FMBUtils::AttachItemToSocket(CurrentWeapon, Character->GetMesh(), CurrentWeaponAnimationsData.WeaponEquipSocketName);
    /*for (auto WeaponClass : WeaponClasses)
    {
        auto Weapon = GetWorld()->SpawnActor<AFMBBaseWeapon>(WeaponClass);
        if (!Weapon) continue;

        Weapon->SetOwner(Character);

        //Weapons.Add(Weapon);

        if (Weapon->GetRootComponent())
        {
            Weapon->GetRootComponent()->SetVisibility(false, true);
        }
        FMBUtils::AttachItemToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
    }*/
}

/*void UFMBBaseWeaponComponent::EquipWeapon()
{
    if (!Character) return;

    bIsEquipAnimInProgress = true;
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
}*/

/*void UFMBBaseWeaponComponent::NextWeapon()
{
    if (!CanEquip() || !CanAttack()) return;
    int32 CurrentWeaponIndexTemp = CurrentWeaponIndex;
    ++CurrentWeaponIndexTemp;
    CurrentWeaponIndex = (CurrentWeaponIndexTemp) % Weapons.Num();
    EquipWeapon();
}*/

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
    if (WeaponsAnimationsData.Contains(EWeaponType::EWT_RedSword))
    {
        InitAnimation(WeaponsAnimationsData[EWeaponType::EWT_RedSword]);
    }
    if (WeaponsAnimationsData.Contains(EWeaponType::EWT_YellowSword))
    {
        InitAnimation(WeaponsAnimationsData[EWeaponType::EWT_YellowSword]);
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
        UE_LOG(LogFMBWeaponComponent, Error, TEXT("Rolling Finished anim notify don't set"));
        checkNoEntry();
    }
    /*const auto ChangeEquipWeapon = FMBAnimUtils::FindNotifyByClass<UFMBChangeEquipWeaponAnimNotify>(WeaponAnimationData.Equip);
    if (ChangeEquipWeapon)
    {
        ChangeEquipWeapon->OnNotify.AddUObject(this, &UFMBBaseWeaponComponent::OnChangeEquipWeapon);
    }
    else
    {
        UE_LOG(LogFMBWeaponComponent, Error, TEXT("Change weapon anim notify don't set"));
        checkNoEntry();
    }
    const auto EquipFinished = FMBAnimUtils::FindNotifyByClass<UFMBAnimFinishedNotify>(WeaponAnimationData.Equip);
    if (EquipFinished)
    {
        EquipFinished->OnNotify.AddUObject(this, &UFMBBaseWeaponComponent::OnEquipFinished);
    }
    else
    {
        UE_LOG(LogFMBWeaponComponent, Error, TEXT("Equip weapon anim notify don't set"));
        checkNoEntry();
    }*/

    CheckAttackAnimNotifyState(WeaponAnimationData.FastAttack);
    CheckAttackAnimNotifyState(WeaponAnimationData.StrongAttack);
}

void UFMBBaseWeaponComponent::CheckAttackAnimNotifyState(UAnimMontage* Animation)
{
    const auto AttackEvent = FMBAnimUtils::FindNotifyByClass<UFMBFinishedAnimNotifyState>(Animation);
    const auto AttackAnimEvent = FMBAnimUtils::FindNotifyByClass<UFMBAnimFinishedNotify>(Animation);
    if (AttackEvent && AttackAnimEvent)
    {
        AttackEvent->OnNotifyStateBegin.AddUObject(this, &UFMBBaseWeaponComponent::OnAttackNotifyStateBegin);
        AttackEvent->OnNotifyStateEnd.AddUObject(this, &UFMBBaseWeaponComponent::OnAttackNotifyStateEnd);

        AttackAnimEvent->OnNotify.AddUObject(this, &UFMBBaseWeaponComponent::OnAttackNotifyAnimEnd);
    }
    else
    {
        UE_LOG(LogFMBWeaponComponent, Error, TEXT("Attack finished weapon anim notifies don't set"));
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

/*void UFMBBaseWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp)
{
    if (!Character || Character->GetMesh() != MeshComp) return;
    bIsEquipAnimInProgress = false;
}*/

/*void UFMBBaseWeaponComponent::OnChangeEquipWeapon(USkeletalMeshComponent* MeshComp)
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
}*/

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

    if (!MovementComponent /*|| MovementComponent->IsFalling()*/ || !(MovementComponent->CanRolling())) return false;

    return true;
}

bool UFMBBaseWeaponComponent::CanEquip() const
{
    return !bIsEquipAnimInProgress;
}

bool UFMBBaseWeaponComponent::GetCurrentWeaponUIData(FWeaponUIData& WeaponUIData) const
{
    if (CurrentWeapon)
    {
        WeaponUIData = CurrentWeapon->GetWeaponUIData();
        return true;
    }
    return false;
}

// work if u have only 2 weapons //
/*bool UFMBBaseWeaponComponent::GetArmoryWeaponUIData(FWeaponUIData& WeaponUIData) const
{
    if (ArmoryWeapon)
    {
        WeaponUIData = ArmoryWeapon->GetWeaponUIData();
        return true;
    }
    return false;
}*/
