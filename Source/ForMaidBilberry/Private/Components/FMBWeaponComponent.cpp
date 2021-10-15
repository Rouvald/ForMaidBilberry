// For Maid Bilberry Game. All Rights Recerved


#include "Components/FMBWeaponComponent.h"
#include "FMBBaseCharacter.h"
#include "GameFramework/Character.h"
#include "Weapon/FMBBaseWeapon.h"
#include "Animation/FMBAttackFinishedNotify.h"
#include "Animation/FMBChangeEquipWeaponAnimNotify.h"
#include "Animation/FMBEquipFinishedAnimNotify.h"
#include "Components/FMBCharacterMovementComponent.h"

DECLARE_LOG_CATEGORY_CLASS(BaseWeaponComponentLog, All, All);

UFMBWeaponComponent::UFMBWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UFMBWeaponComponent::BeginPlay()
{
    Super::BeginPlay();
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

void UFMBWeaponComponent::InitAnimation()
{
    const auto ChangeEquipWeapon = FindNotifyByClass<UFMBChangeEquipWeaponAnimNotify>(EquipAnimMontage);
    if (ChangeEquipWeapon)
    {
        ChangeEquipWeapon->OnChangeEquipWeapon.AddUObject(this, &UFMBWeaponComponent::OnChangeEquipWeapon);
    }
    const auto EquipFinished = FindNotifyByClass<UFMBEquipFinishedAnimNotify>(EquipAnimMontage);
    if (EquipFinished)
    {
        EquipFinished->OnEquipFinishedNotify.AddUObject(this, &UFMBWeaponComponent::OnEquipFinished);
    }
    const auto FastMeleeAttackEvent = FindNotifyByClass<UFMBAttackFinishedNotify>(FastMeleeAttackAnimMontage);
    if (FastMeleeAttackEvent)
    {
        FastMeleeAttackEvent->OnAttackFinishedNotify.AddUObject(this, &UFMBWeaponComponent::OnAttackFinished);
    }
    const auto StrongMeleeAttackEvent = FindNotifyByClass<UFMBAttackFinishedNotify>(StrongMeleeAttackAnimMontage);
    if (StrongMeleeAttackEvent)
    {
        StrongMeleeAttackEvent->OnAttackFinishedNotify.AddUObject(this, &UFMBWeaponComponent::OnAttackFinished);
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

    if (Character->GetVelocity().Z != 0.0f) return AttackAnimInProgress;

    if (EquipAnimInProgress) return AttackAnimInProgress;

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
