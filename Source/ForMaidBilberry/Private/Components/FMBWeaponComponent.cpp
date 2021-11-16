// For Maid Bilberry Game. All Rights Recerved


#include "Components/FMBWeaponComponent.h"
#include "FMBBaseCharacter.h"
#include "Animation/FMBAnimFinishedNotify.h"
#include "GameFramework/Character.h"
#include "Weapon/FMBBaseWeapon.h"
#include "Animation/FMBAnimUtils.h"
#include "Animation/FMBChangeEquipWeaponAnimNotify.h"
#include "Components/FMBCharacterMovementComponent.h"
#include "Components/FMBHealthComponent.h"
#include "FMBUtils.h"
#include "FMBCoreTypes.h"

DECLARE_LOG_CATEGORY_CLASS(BaseWeaponComponentLog, All, All);

UFMBWeaponComponent::UFMBWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UFMBWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    checkf(Weapons.Num() <=0, TEXT("Character don't have weapon"));
    CurrentWeaponIndex = 0;

    CheckWeaponAnimationsData();
    SpawnWeapons();

    EquipWeapon(CurrentWeaponIndex);
}

void UFMBWeaponComponent::SpawnWeapons()
{
    if (!GetWorld()) return;

    const auto Character = GetCharacter();
    if (!Character) return;

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
        AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
    }
}

void UFMBWeaponComponent::AttachWeaponToSocket(AFMBBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& WeaponSocket) const
{
    if (!Weapon || !SceneComponent) return;

    const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, WeaponSocket);
}

void UFMBWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
    const auto Character = GetCharacter();
    if (!Character) return;

    EquipAnimInProgress = true;
    if (CurrentWeapon)
    {
        PlayAnimMontage(CurrentWeaponAnimationsData.Equip);
    }
    else
    {
        PlayAnimMontage(WeaponsAnimationsData[EWeaponType::RedSword].Equip);
    }
    //==================================================
    //UE_LOG(BaseWeaponComponentLog, Display, TEXT("Equip"));
    //==================================================
}

void UFMBWeaponComponent::NextWeapon()
{
    if (!CanEquip() || !CanAttack()) return;
    CurrentWeaponIndex = (++CurrentWeaponIndex) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}

void UFMBWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    CurrentWeapon = nullptr;
    for (const auto Weapon : Weapons)
    {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->Destroy();
    }
    Weapons.Empty();

    Super::EndPlay(EndPlayReason);
}

void UFMBWeaponComponent::FastMeleeAttack()
{
    if (!CanDoAttack()) return;

    CurrentWeapon->MeleeAttack(EChooseAttack::FastAttack);

    AttackAnimInProgress = true;
    StopMovement();
    PlayAnimMontage(CurrentWeaponAnimationsData.FastAttack);
    //==================================================
    //UE_LOG(BaseWeaponComponentLog, Display, TEXT("Fast Attack make"));
    //==================================================
}

void UFMBWeaponComponent::StrongMeleeAttack()
{
    if (!CanDoAttack()) return;

    CurrentWeapon->MeleeAttack(EChooseAttack::StrongAttack);

    AttackAnimInProgress = true;
    StopMovement();
    PlayAnimMontage(CurrentWeaponAnimationsData.StrongAttack);
    //==================================================
    //UE_LOG(BaseWeaponComponentLog, Display, TEXT("Strong Attack make"));
    //==================================================
}

bool UFMBWeaponComponent::CanDoAttack() const
{
    if (!CanAttack()) return false;
    if (!CurrentWeapon) return false;

    const auto MovementComponent = FMBUtils::GetFMBPlayerComponent<UFMBCharacterMovementComponent>(GetOwner());
    if (!MovementComponent || MovementComponent->IsFalling()) return false;

    const auto HealthComponent = FMBUtils::GetFMBPlayerComponent<UFMBHealthComponent>(GetOwner());
    if (!HealthComponent || !(HealthComponent->SpendStamina(EStaminaSpend::StrongAttack))) return false;

    return true;
}

void UFMBWeaponComponent::PlayAnimMontage(UAnimMontage* Animation) const
{
    const auto Character = GetCharacter();
    if (!Character) return;
    Character->PlayAnimMontage(Animation);
}

void UFMBWeaponComponent::CheckWeaponAnimationsData()
{
    if (WeaponsAnimationsData.Contains(EWeaponType::RedSword))
    {
        InitAnimation(WeaponsAnimationsData[EWeaponType::RedSword]);
    }
    if (WeaponsAnimationsData.Contains(EWeaponType::YellowSword))
    {
        InitAnimation(WeaponsAnimationsData[EWeaponType::YellowSword]);
    }
}

void UFMBWeaponComponent::InitAnimation(const FWeaponAnimationsData& WeaponAnimationData)
{
    const auto RollingEvent = FMBAnimUtils::FindNotifyByClass<UFMBAnimFinishedNotify>(WeaponAnimationData.Roll);
    const auto MovementComponent = FMBUtils::GetFMBPlayerComponent<UFMBCharacterMovementComponent>(GetOwner());
    if (MovementComponent || RollingEvent)
    {
        RollingEvent->OnNotify.AddUObject(MovementComponent, &UFMBCharacterMovementComponent::OnRollingFinished);
    }
    else
    {
        UE_LOG(BaseWeaponComponentLog, Error, TEXT("Rolling Finished anim notify don't set"));
        checkNoEntry();
    }
    const auto ChangeEquipWeapon = FMBAnimUtils::FindNotifyByClass<UFMBChangeEquipWeaponAnimNotify>(WeaponAnimationData.Equip);
    if (ChangeEquipWeapon)
    {
        ChangeEquipWeapon->OnNotify.AddUObject(this, &UFMBWeaponComponent::OnChangeEquipWeapon);
    }
    else
    {
        UE_LOG(BaseWeaponComponentLog, Error, TEXT("Change weapon anim notify don't set"));
        checkNoEntry();
    }
    const auto EquipFinished = FMBAnimUtils::FindNotifyByClass<UFMBAnimFinishedNotify>(WeaponAnimationData.Equip);
    if (EquipFinished)
    {
        EquipFinished->OnNotify.AddUObject(this, &UFMBWeaponComponent::OnEquipFinished);
    }
    else
    {
        UE_LOG(BaseWeaponComponentLog, Error, TEXT("Equip weapon anim notify don't set"));
        checkNoEntry();
    }

    CheckAttackFinishedAnimNotify(WeaponAnimationData.FastAttack);
    CheckAttackFinishedAnimNotify(WeaponAnimationData.StrongAttack);
}

void UFMBWeaponComponent::CheckAttackFinishedAnimNotify(UAnimMontage* Animation)
{
    const auto AttackEvent = FMBAnimUtils::FindNotifyByClass<UFMBAnimFinishedNotify>(Animation);
    if (AttackEvent)
    {
        AttackEvent->OnNotify.AddUObject(this, &UFMBWeaponComponent::OnAttackFinished);
    }
    else
    {
        UE_LOG(BaseWeaponComponentLog, Error, TEXT("Attack Finished weapon anim notify don't set"));
        checkNoEntry();
    }
}

void UFMBWeaponComponent::OnAttackFinished(USkeletalMeshComponent* MeshComp)
{
    const auto HealthComponent = FMBUtils::GetFMBPlayerComponent<UFMBHealthComponent>(GetOwner());
    if (!HealthComponent) return;

    HealthComponent->StartHealStaminaTimer();

    StopDrawTrace();
}

void UFMBWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp)
{
    const auto Character = GetCharacter();
    if (!Character || Character->GetMesh() != MeshComp) return;

    EquipAnimInProgress = false;
}

void UFMBWeaponComponent::OnChangeEquipWeapon(USkeletalMeshComponent* MeshComp)
{
    if (CurrentWeaponIndex < 0 || CurrentWeaponIndex > Weapons.Num())
    {
        UE_LOG(BaseWeaponComponentLog, Display, TEXT("Incorrect Weaon Index"));
        return;
    }
    const auto Character = GetCharacter();
    if (!Character || Character->GetMesh() != MeshComp) return;

    if (CurrentWeapon)
    {
        StopDrawTrace();
        if (CurrentWeapon->GetRootComponent())
        {
            CurrentWeapon->GetRootComponent()->SetVisibility(false, true);
        }
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
    }
    CurrentWeapon = Weapons[CurrentWeaponIndex];
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

    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), CurrentWeaponAnimationsData.WeaponEquipSocketName);
}

void UFMBWeaponComponent::StopDrawTrace()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->StopDrawTrace();
        AttackAnimInProgress = false;
    }
    StartMovement();
}

bool UFMBWeaponComponent::CanAttack() const
{
    const auto Character = GetCharacter();
    if (!Character) return AttackAnimInProgress;

    if (EquipAnimInProgress) return AttackAnimInProgress;

    const auto MovementComponent = FMBUtils::GetFMBPlayerComponent<UFMBCharacterMovementComponent>(GetOwner());
    if (!MovementComponent) return AttackAnimInProgress;
    if (!(MovementComponent->CanRolling())) return AttackAnimInProgress;

    return !AttackAnimInProgress;
}

bool UFMBWeaponComponent::CanEquip() const
{
    return !EquipAnimInProgress;
}

void UFMBWeaponComponent::StartMovement() const
{
    const auto MovementComponent = FMBUtils::GetFMBPlayerComponent<UFMBCharacterMovementComponent>(GetOwner());
    if (!MovementComponent) return;

    if (!AttackAnimInProgress)
    {
        MovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);
    }
}

void UFMBWeaponComponent::StopMovement() const
{
    const auto MovementComponent = FMBUtils::GetFMBPlayerComponent<UFMBCharacterMovementComponent>(GetOwner());
    if (!MovementComponent) return;

    if (AttackAnimInProgress)
    {
        MovementComponent->SetMovementMode(EMovementMode::MOVE_None);
    }
}

AFMBBaseCharacter* UFMBWeaponComponent::GetCharacter() const
{
    const auto Character = Cast<AFMBBaseCharacter>(GetOwner());
    if (!Character) return nullptr;

    return Character;
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
