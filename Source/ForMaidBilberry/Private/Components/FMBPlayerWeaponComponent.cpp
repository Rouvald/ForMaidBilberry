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

    InitWeaponComponent();

    Weapons.Init(nullptr, MaxWeapons);
    PickUps.Init(nullptr, MaxPickUps);

    // TestLogs();
}

void UFMBPlayerWeaponComponent::InitWeaponComponent()
{
    if (Character)
    {
        ItemInteractionComponent = Character->FindComponentByClass<UFMBItemInteractionComponent>();
        if (WeaponsAnimationsData.Contains(EWeaponType::EWT_NoWeapon))
        {
            CurrentWeaponAnimationsData = WeaponsAnimationsData[EWeaponType::EWT_NoWeapon];
        }
    }
}

void UFMBPlayerWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    DestroyWeapons();
    // DestroyCurrentShield();
    DestroyPickUps();

    TestLogs();

    Super::EndPlay(EndPlayReason);
}

void UFMBPlayerWeaponComponent::DestroyWeapons()
{
    if (CurrentWeapon)
    {
        OnItemSelected.Broadcast(CurrentWeaponIndex, CurrentWeapon->GetItemData(), false);
        CurrentWeapon = nullptr;
    }
    for (int8 Index = 0; Index < MaxWeapons; ++Index)
    {
        if (Weapons[Index])
        {
            OnItemIconVisibility.Broadcast(Index, Weapons[Index]->GetItemData(), false);
            Weapons[Index]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
            Weapons[Index]->Destroy();
        }
    }
    Weapons.Empty();
}

/*void UFMBPlayerWeaponComponent::DestroyCurrentShield()
{
    if (CurrentShield)
    {
        CurrentShield->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        if (CurrentShield->Destroy())
        {
            CurrentShield = nullptr;
        }
    }
}*/

void UFMBPlayerWeaponComponent::DestroyPickUps()
{
    if (CurrentPickUp)
    {
        OnItemSelected.Broadcast(CurrentPickUpIndex, CurrentPickUp->GetItemData(), false);
        CurrentPickUp = nullptr;
    }
    for (int8 Index = 0; Index < MaxPickUps; ++Index)
    {
        if (PickUps[Index])
        {
            OnItemIconVisibility.Broadcast(Index, PickUps[Index]->GetItemData(), false);
            OnItemCountVisible.Broadcast(Index, PickUps[Index]->GetItemData(), false);
            PickUps[Index]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
            PickUps[Index]->Destroy();
        }
    }
    PickUps.Empty();
}

/*void UFMBPlayerWeaponComponent::EquipItems()
{
    Super::EquipItems();
    // SpawnShields();
}*/

/*void UFMBPlayerWeaponComponent::SpawnEquipShields()
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
}*/

/*void UFMBPlayerWeaponComponent::OnStartBlock()
{
    if (!CurrentShield) return;

    // CurrentShield->StartBlocking();
    bIsShieldBlocking = true;
}*/

/*void UFMBPlayerWeaponComponent::OnStopBlock()
{
    // CurrentShield->StopBlocking();
    bIsShieldBlocking = false;
}*/

void UFMBPlayerWeaponComponent::UsePickUp()
{
    if (!Character || !CurrentPickUp) return;

    CurrentPickUp->UsePickUp();
}

void UFMBPlayerWeaponComponent::PickUpWasUsed()
{
    if (!Character || !CurrentPickUp) return;

    CurrentPickUp->ChangeItemCount(false);
    OnItemCountChange.Broadcast(CurrentPickUpIndex, CurrentPickUp->GetItemData());

    if (CurrentPickUp->GetItemCount() < 1)
    {
        DestroyPickUps();
    }
}

void UFMBPlayerWeaponComponent::GetPickupItem(AFMBBaseItem* Item)
{
    if (!Item) return;

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
        const auto Index{Weapons.Find(nullptr)};
        if (Index != INDEX_NONE)
        {
            Weapons[Index] = EquippedWeapon;
            FMBUtils::AttachItemToSocket(EquippedWeapon, Character->GetMesh(), WeaponArmorySocketName);
            EquippedWeapon->OnItemStateChanged.Broadcast(EItemState::EIS_PickedUp);
            OnItemPickedUp.Broadcast(Index, EquippedWeapon->GetItemData());
            // OnItemIconVisibility.Broadcast(Index, EquippedWeapon->GetItemData(), true);
        }
    }
    else
    {
        if (WeaponsAnimationsData.Contains(EquippedWeapon->GetWeaponType()))
        {
            CurrentWeaponAnimationsData = WeaponsAnimationsData[EquippedWeapon->GetWeaponType()];
        }
        FMBUtils::AttachItemToSocket(EquippedWeapon, Character->GetMesh(), CurrentWeaponAnimationsData.WeaponEquipSocketName);
        CurrentWeapon = EquippedWeapon;
        Weapons[CurrentWeaponIndex] = EquippedWeapon;
        CurrentWeapon->OnItemStateChanged.Broadcast(EItemState::EIS_Equipped);

        OnItemPickedUp.Broadcast(CurrentWeaponIndex, EquippedWeapon->GetItemData());
        // OnItemIconVisibility.Broadcast(CurrentWeaponIndex, EquippedWeapon->GetItemData(), true);
        OnItemSelected.Broadcast(CurrentWeaponIndex, EquippedWeapon->GetItemData(), true);
    }
    TestLogs();
}

void UFMBPlayerWeaponComponent::EquipPickUp(AFMBBasePickUp* EquippedPickUp)
{
    if (!EquippedPickUp || !Character) return;

    if (CurrentPickUp)
    {
        if (CurrentPickUp->GetPickUpType() == EquippedPickUp->GetPickUpType())
        {
            EquippedPickUp->Destroy();
            CurrentPickUp->ChangeItemCount(true);
            OnItemCountChange.Broadcast(CurrentPickUpIndex, CurrentPickUp->GetItemData());
        }
        else
        {
            for (int8 Index = 0; Index < MaxPickUps; ++Index)
            {
                if (!PickUps[Index])
                {
                    PickUps[Index] = EquippedPickUp;
                    EquippedPickUp->SetOwner(Character);
                    FMBUtils::AttachItemToSocket(EquippedPickUp, Character->GetMesh(), PickUpEquipSocketName);
                    EquippedPickUp->OnItemStateChanged.Broadcast(EItemState::EIS_Equipped);
                    OnItemPickedUp.Broadcast(Index, EquippedPickUp->GetItemData());
                    OnItemCountChange.Broadcast(Index, EquippedPickUp->GetItemData());
                    break;
                }
            }
        }
    }
    else
    {
        EquippedPickUp->SetOwner(Character);
        FMBUtils::AttachItemToSocket(EquippedPickUp, Character->GetMesh(), PickUpEquipSocketName);
        CurrentPickUp = EquippedPickUp;
        EquippedPickUp->OnItemStateChanged.Broadcast(EItemState::EIS_Equipped);
        OnItemPickedUp.Broadcast(CurrentPickUpIndex, EquippedPickUp->GetItemData());
        OnItemSelected.Broadcast(CurrentPickUpIndex, EquippedPickUp->GetItemData(), true);
        OnItemCountChange.Broadcast(CurrentPickUpIndex, EquippedPickUp->GetItemData());
    }
    if (CurrentPickUp) UE_LOG(LogFMBPlayerWeaponComponent, Warning, TEXT("CurrentPickUp, PickUps.Num == %d"), PickUps.Num());
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

    if (Weapons.Find(nullptr) == INDEX_NONE)
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
    /* todo:*/
    // OnItemThrow.Broadcast(?Index?, DropItem);
    DropItem->ThrowWeapon();
}

AFMBPlayerCharacter* UFMBPlayerWeaponComponent::GetPlayerCharacter() const
{
    return Cast<AFMBPlayerCharacter>(GetOwner());
}

void UFMBPlayerWeaponComponent::TestLogs()
{
    UE_LOG(LogFMBPlayerWeaponComponent, Warning, TEXT("Weapons.Num = %d"), Weapons.Num());
    for (const auto Weapon : Weapons)
    {
        if (Weapon)
        {
            UE_LOG(LogFMBPlayerWeaponComponent, Warning, TEXT("Weapon = %s"), *Weapon->GetName());
        }
        else
        {
            UE_LOG(LogFMBPlayerWeaponComponent, Warning, TEXT("Weapon == nullptr"));
        }
    }
}
