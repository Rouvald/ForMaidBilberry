// For Maid Bilberry Game. All Rights Reserved

#include "Components/FMBPlayerWeaponComponent.h"

#include "Player/FMBPlayerCharacter.h"
#include "FMBBasePickUp.h"
#include "FMBBaseWeapon.h"
#include "FMBItemInteractionComponent.h"
#include "Items/Weapon/FMBBaseShield.h"
#include "Player/FMBBaseCharacter.h"
#include "FMBUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBPlayerWeaponComponent, All, All)

void UFMBPlayerWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    if (Character && Character->IsPlayerControlled())
    {
        ItemInteractionComponent = Character->FindComponentByClass<UFMBItemInteractionComponent>();
    }
}

void UFMBPlayerWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    DestroyWeapons();
    DestroyCurrentShield();
    DestroyCurrentPickUp();

    Super::EndPlay(EndPlayReason);
}

void UFMBPlayerWeaponComponent::DestroyWeapons()
{
    CurrentWeapon = nullptr;
    for (const auto Weapon : Weapons)
    {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->Destroy();
    }
    Weapons.Empty();
}

void UFMBPlayerWeaponComponent::DestroyCurrentShield() const
{
    if (CurrentShield)
    {
        CurrentShield->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        CurrentShield->Destroy();
    }
}

void UFMBPlayerWeaponComponent::DestroyCurrentPickUp() const
{
    if (CurrentPickUp)
    {
        CurrentPickUp->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        CurrentPickUp->Destroy();
    }
}

void UFMBPlayerWeaponComponent::SpawnItems()
{
    Super::SpawnItems();
    // SpawnShields();
}

void UFMBPlayerWeaponComponent::SpawnShields()
{
    CurrentShield = GetWorld()->SpawnActor<AFMBBaseShield>(ShieldClass);
    if (!CurrentShield)
    {
        UE_LOG(LogFMBPlayerWeaponComponent, Display, TEXT("Error: spawn shield"));
        return;
    }

    CurrentShield->SetOwner(Character);

    if (CurrentShield->GetRootComponent())
    {
        CurrentShield->GetRootComponent()->SetVisibility(true, true);
    }
    FMBUtils::AttachItemToSocket(CurrentShield, Character->GetMesh(), ShieldEquipSocketName);
}

void UFMBPlayerWeaponComponent::OnStartBlock()
{
    if (!CurrentShield) return;

    // CurrentShield->StartBlocking();
    bIsShieldBlocking = true;
}

void UFMBPlayerWeaponComponent::OnStopBlock()
{
    // CurrentShield->StopBlocking();
    bIsShieldBlocking = false;
}

void UFMBPlayerWeaponComponent::GetPickupItem(AFMBBaseItem* Item)
{
    const auto Weapon = Cast<AFMBBaseWeapon>(Item);
    if (Weapon)
    {
        SwapWeapon(Weapon);
        return;
    }
    const auto PickUp = Cast<AFMBBasePickUp>(Item);
    if (PickUp)
    {
        SwapPickUp(PickUp);
        // return;
    }
}

void UFMBPlayerWeaponComponent::EquipWeapon(AFMBBaseWeapon* EquippedWeapon)
{
    if (!EquippedWeapon || !Character) return;

    EquippedWeapon->SetOwner(Character);
    if (CurrentWeapon)
    {
        FMBUtils::AttachItemToSocket(EquippedWeapon, Character->GetMesh(), WeaponArmorySocketName);
        EquippedWeapon->OnItemStateChanged.Broadcast(EItemState::EIS_PickedUp);
        Weapons.AddUnique(EquippedWeapon);
        OnWeaponPickedUp.Broadcast((Weapons.Num() - 1), EquippedWeapon->GetItemData());
    }
    else
    {
        if (WeaponsAnimationsData.Contains(EquippedWeapon->GetWeaponType()))
        {
            CurrentWeaponAnimationsData = WeaponsAnimationsData[EquippedWeapon->GetWeaponType()];
        }
        FMBUtils::AttachItemToSocket(EquippedWeapon, Character->GetMesh(), CurrentWeaponAnimationsData.WeaponEquipSocketName);
        CurrentWeapon = EquippedWeapon;
        CurrentWeapon->OnItemStateChanged.Broadcast(EItemState::EIS_Equipped);
        OnWeaponSelected.Broadcast(CurrentWeaponIndex);
        if (Weapons.Num() == 0)
        {
            Weapons.AddUnique(EquippedWeapon);
        }
        else
        {
            Weapons[CurrentWeaponIndex] = EquippedWeapon;
        }
        OnWeaponPickedUp.Broadcast(CurrentWeaponIndex, EquippedWeapon->GetItemData());
    }
}

void UFMBPlayerWeaponComponent::EquipPickUp(AFMBBasePickUp* EquippedPickUp)
{
    if (!EquippedPickUp || !Character) return;

    if (CurrentPickUp && (CurrentPickUp->GetPickUpType() == EquippedPickUp->GetPickUpType()))
    {
        EquippedPickUp->Destroy();
        CurrentPickUp->ChangeItemCount(true);
    }
    else
    {
        EquippedPickUp->SetOwner(Character);
        FMBUtils::AttachItemToSocket(EquippedPickUp, Character->GetMesh(), PickUpEquipSocketName);

        CurrentPickUp = EquippedPickUp;
        CurrentPickUp->OnItemStateChanged.Broadcast(EItemState::EIS_Equipped);
        OnPickUpPickedUp.Broadcast(CurrentPickUp->GetItemData());
    }
    OnPickUpCountChange.Broadcast(CurrentPickUp->GetItemCount());
}

void UFMBPlayerWeaponComponent::SwapPickUp(AFMBBasePickUp* EquippedPickUp)
{
    if (!Character || !ItemInteractionComponent) return;

    if (CurrentPickUp && (CurrentPickUp->GetPickUpType() != EquippedPickUp->GetPickUpType()))
    {
        DropItem(CurrentPickUp);
    }
    EquipPickUp(EquippedPickUp);
    ItemInteractionComponent->ClearHitItem();
}

void UFMBPlayerWeaponComponent::SwapWeapon(AFMBBaseWeapon* EquippedWeapon)
{
    if (!Character || !ItemInteractionComponent) return;

    if (Weapons.Num() >= MaxWeapons)
    {
        DropItem(CurrentWeapon);
        ClearCurrentWeapon();
    }
    EquipWeapon(EquippedWeapon);
    ItemInteractionComponent->ClearHitItem();
}

void UFMBPlayerWeaponComponent::ClearCurrentWeapon()
{
    CurrentWeapon = nullptr;
    Weapons[CurrentWeaponIndex] = nullptr;
}

void UFMBPlayerWeaponComponent::DropItem(AFMBBaseItem* DropItem) const
{
    if (!DropItem) return;

    const auto ItemMesh = DropItem->GetItemMesh();
    if (!ItemMesh) return;

    const FDetachmentTransformRules DetachmentTransformRules{EDetachmentRule::KeepWorld, true};
    ItemMesh->DetachFromComponent(DetachmentTransformRules);

    DropItem->OnItemStateChanged.Broadcast(EItemState::EIS_Falling);
    DropItem->ThrowWeapon();
}

AFMBPlayerCharacter* UFMBPlayerWeaponComponent::GetPlayerCharacter() const
{
    return Cast<AFMBPlayerCharacter>(GetOwner());
}
