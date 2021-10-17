// For Maid Bilberry Game. All Rights Recerved


#include "Components/FMBWeaponComponent.h"
#include "FMBBaseCharacter.h"
#include "Animation/FMBAnimFinishedNotify.h"
#include "GameFramework/Character.h"
#include "Weapon/FMBBaseWeapon.h"
#include "Animation/FMBAnimUtils.h"
#include "Animation/FMBChangeEquipWeaponAnimNotify.h"
#include "Components/FMBCharacterMovementComponent.h"

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

    InitAnimation();

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

        Weapon->SetActorHiddenInGame(true);
        AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
    }
}

void UFMBWeaponComponent::AttachWeaponToSocket(AFMBBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& WeaponSocket)
{
    if (!Weapon || !SceneComponent) return;

    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, WeaponSocket);
}

void UFMBWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
    const auto Character = GetCharacter();
    if (!Character) return;

    EquipAnimInProgress = true;
    PlayAnimMontage(EquipAnimMontage);
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
    for (auto Weapon : Weapons)
    {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->Destroy();
    }
    Weapons.Empty();

    Super::EndPlay(EndPlayReason);
}

void UFMBWeaponComponent::FastMeleeAttack()
{
    if (!CanAttack()) return;
    if (!CurrentWeapon) return;
    const auto Character = GetCharacter();
    if (!Character || Character->GetVelocity().Z != 0.0f) return;

    CurrentWeapon->FastMeleeAttack();

    AttackAnimInProgress = true;
    StopMovement();
    PlayAnimMontage(FastMeleeAttackAnimMontage);
}

void UFMBWeaponComponent::StrongMeleeAttack()
{
    if (!CanAttack()) return;
    if (!CurrentWeapon) return;
    const auto Character = GetCharacter();
    if (!Character || Character->GetVelocity().Z != 0.0f) return;

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

void UFMBWeaponComponent::InitAnimation()
{
    const auto ChangeEquipWeapon = FMBAnimUtils::FindNotifyByClass<UFMBChangeEquipWeaponAnimNotify>(EquipAnimMontage);
    if (ChangeEquipWeapon)
    {
        ChangeEquipWeapon->OnNotify.AddUObject(this, &UFMBWeaponComponent::OnChangeEquipWeapon);
    }
    else
    {
        UE_LOG(BaseWeaponComponentLog, Error, TEXT("Change weapon anim notify don't set"));
        checkNoEntry();
    }
    const auto EquipFinished = FMBAnimUtils::FindNotifyByClass<UFMBAnimFinishedNotify>(EquipAnimMontage);
    if (EquipFinished)
    {
        EquipFinished->OnNotify.AddUObject(this, &UFMBWeaponComponent::OnEquipFinished);
    }
    else
    {
        UE_LOG(BaseWeaponComponentLog, Error, TEXT("Equip weapon anim notify don't set"));
        checkNoEntry();
    }
    CheckAttackFinishedAnimNotify(FastMeleeAttackAnimMontage);
    CheckAttackFinishedAnimNotify(StrongMeleeAttackAnimMontage);
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
        CurrentWeapon->SetActorHiddenInGame(true);
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
    }
    CurrentWeapon = Weapons[CurrentWeaponIndex];
    if (CurrentWeapon->IsHidden())
    {
        CurrentWeapon->SetActorHiddenInGame(false);
    }
    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
}

bool UFMBWeaponComponent::CanAttack() const
{
    const auto Character = GetCharacter();
    if (!Character) return AttackAnimInProgress;

    if (EquipAnimInProgress) return AttackAnimInProgress;

    const auto MovementComponent = Cast<UFMBCharacterMovementComponent>(GetMovementComponent());
    if (!MovementComponent) return AttackAnimInProgress;
    if (!(MovementComponent->CanRolling())) return AttackAnimInProgress;

    return !AttackAnimInProgress;
}

bool UFMBWeaponComponent::CanEquip() const
{
    return !EquipAnimInProgress;
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

AFMBBaseCharacter* UFMBWeaponComponent::GetCharacter() const
{
    const auto Character = Cast<AFMBBaseCharacter>(GetOwner());
    if (!Character) return nullptr;

    return Character;
}

UCharacterMovementComponent* UFMBWeaponComponent::GetMovementComponent() const
{
    const auto Character = GetCharacter();
    if (!Character) return nullptr;

    const auto MovementComponent = Cast<UCharacterMovementComponent>(Character->GetMovementComponent());
    if (!MovementComponent) return nullptr;

    return MovementComponent;
}
