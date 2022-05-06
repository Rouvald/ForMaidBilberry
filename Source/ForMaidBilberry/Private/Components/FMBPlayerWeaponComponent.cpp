// For Maid Bilberry Game. All Rights Reserved

#include "Components/FMBPlayerWeaponComponent.h"
#include "Player/FMBPlayerCharacter.h"
#include "FMBBasePickUp.h"
#include "FMBBaseWeapon.h"
#include "FMBItemInteractionComponent.h"
#include "FMBUtils.h"
#include "Animation/FMBAnimUtils.h"
#include "Animation/FMBChangeEquipWeaponAnimNotify.h"
#include "Animation/FMBAnimFinishedNotify.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBPlayerWeaponComponent, All, All)

void UFMBPlayerWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    if (Character)
    {
        ItemInteractionComponent = Character->FindComponentByClass<UFMBItemInteractionComponent>();
    }

    checkf(MaxWeapons > 0, TEXT("MaxWeapons <= 0"));
    Weapons.Init(nullptr, MaxWeapons);
    CurrentWeaponIndex = 0;
    CurrentPickUpIndex = 0;

    OnItemSelected.Broadcast(0, EItemType::EIT_Weapon, true);
    // TestLogs();
}

void UFMBPlayerWeaponComponent::InitAnimation(const FWeaponAnimationsData& WeaponAnimationData)
{
    Super::InitAnimation(WeaponAnimationData);
    if (!Character) return;

    const auto ChangeEquipWeapon = FMBAnimUtils::FindNotifyByClass<UFMBChangeEquipWeaponAnimNotify>(WeaponAnimationData.Equip);
    if (ChangeEquipWeapon)
    {
        ChangeEquipWeapon->OnNotify.AddUObject(this, &UFMBPlayerWeaponComponent::OnChangeEquipWeapon);
    }
    else
    {
        UE_LOG(LogFMBPlayerWeaponComponent, Error, TEXT("Change weapon anim notify don't set"));
        checkNoEntry();
    }
    const auto EquipFinished = FMBAnimUtils::FindNotifyByClass<UFMBAnimFinishedNotify>(WeaponAnimationData.Equip);
    if (EquipFinished)
    {
        EquipFinished->OnNotify.AddUObject(this, &UFMBPlayerWeaponComponent::OnEquipFinished);
    }
    else
    {
        UE_LOG(LogFMBPlayerWeaponComponent, Error, TEXT("Equip weapon anim notify don't set"));
        checkNoEntry();
    }
}

/*void UFMBPlayerWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    DropItems();
    /*DestroyWeapons();
    DestroyCurrentItem(CurrentPickUp, CurrentPickUpIndex);#1#
    //TestLogs();
}*/

void UFMBPlayerWeaponComponent::DropItems()
{
    if (CurrentWeapon)
    {
        OnItemSelected.Broadcast(CurrentWeaponIndex, CurrentWeapon->GetItemData().ItemType, false);
        CurrentWeapon = nullptr;
    }
    for (int8 Index = 0; Index < MaxWeapons; ++Index)
    {
        if (Weapons[Index])
        {
            DropItem(Weapons[Index], Index);
        }
    }
    if (CurrentPickUp)
    {
        OnItemSelected.Broadcast(CurrentPickUpIndex, CurrentPickUp->GetItemData().ItemType, false);
        DropItem(CurrentPickUp, CurrentPickUpIndex);
        CurrentPickUp = nullptr;
    }
}

void UFMBPlayerWeaponComponent::DestroyWeapons()
{
    if (CurrentWeapon)
    {
        OnItemSelected.Broadcast(CurrentWeaponIndex, CurrentWeapon->GetItemData().ItemType, false);
        CurrentWeapon = nullptr;
    }
    for (int8 Index = 0; Index < MaxWeapons; ++Index)
    {
        if (Weapons[Index])
        {
            OnItemIconVisibility.Broadcast(Index, Weapons[Index]->GetItemData().ItemType, false);
            Weapons[Index]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
            if (Weapons[Index]->Destroy())
            {
                Weapons[Index] = nullptr;
            }
        }
    }
}

void UFMBPlayerWeaponComponent::DestroyCurrentItem(AFMBBaseItem* DestroyItem, const int8 CurrentItemIndex)
{
    if (!DestroyItem) return;
    OnItemSelected.Broadcast(CurrentItemIndex, DestroyItem->GetItemData().ItemType, false);
    OnItemIconVisibility.Broadcast(CurrentItemIndex, DestroyItem->GetItemData().ItemType, false);
    OnItemCountVisible.Broadcast(CurrentItemIndex, DestroyItem->GetItemData().ItemType, false);
    DestroyItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    ClearCurrentItem(DestroyItem, CurrentItemIndex);
    DestroyItem->Destroy();
}

void UFMBPlayerWeaponComponent::ClearCurrentItem(const AFMBBaseItem* ClearItem, const int8 CurrentItemIndex)
{
    if (!ClearItem) return;
    if (ClearItem->GetItemData().ItemType == EItemType::EIT_Weapon)
    {
        CurrentWeapon = nullptr;
        Weapons[CurrentItemIndex] = nullptr;
    }
    if (ClearItem->GetItemData().ItemType == EItemType::EIT_PickUp)
    {
        CurrentPickUp = nullptr;
    }
}

void UFMBPlayerWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp)
{
    if (!Character || Character->GetMesh() != MeshComp) return;
    bIsEquipAnimInProgress = false;
}

void UFMBPlayerWeaponComponent::OnChangeEquipWeapon(USkeletalMeshComponent* MeshComp)
{
    if (!Character || Character->GetMesh() != MeshComp) return;

    if (CurrentWeaponIndex < 0 || CurrentWeaponIndex > Weapons.Num())
    {
        UE_LOG(LogFMBPlayerWeaponComponent, Warning, TEXT("Incorrect Weapon Index"));
        return;
    }
    if (CurrentWeapon)
    {
        CurrentWeapon->StopDrawTrace();
        CurrentWeapon->OnItemStateChanged.Broadcast(EItemState::EIS_PickedUp);
        FMBUtils::AttachItemToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
    }
    CurrentWeapon = Weapons[CurrentWeaponIndex];
    OnItemSelected.Broadcast(LastCurrentWeaponIndex, EItemType::EIT_Weapon, false);
    const auto ChangeWeaponAnimation = [&](const EItemType ItemType, const EWeaponType WeaponType)
    {
        CurrentWeaponAnimationsData = WeaponsAnimationsData.FindChecked(WeaponType);
        OnItemSelected.Broadcast(CurrentWeaponIndex, ItemType, true);
    };
    if (CurrentWeapon)
    {
        CurrentWeapon->OnItemStateChanged.Broadcast(EItemState::EIS_Equipped);
        ChangeWeaponAnimation(CurrentWeapon->GetItemData().ItemType, CurrentWeapon->GetWeaponType());
        FMBUtils::AttachItemToSocket(CurrentWeapon, Character->GetMesh(), CurrentWeaponAnimationsData.WeaponEquipSocketName);
    }
    else
    {
        ChangeWeaponAnimation(EItemType::EIT_Weapon, EWeaponType::EWT_NoWeapon);
    }
}

/*void UFMBPlayerWeaponComponent::DestroyPickUps()
{
    if (CurrentPickUp)
    {
        OnItemSelected.Broadcast(CurrentPickUpIndex, CurrentPickUp->GetItemData().ItemType, false);
        CurrentPickUp = nullptr;
    }
    for (int8 Index = 0; Index < MaxPickUps; ++Index)
    {
        if (PickUps[Index])
        {
            OnItemIconVisibility.Broadcast(Index, PickUps[Index]->GetItemData().ItemType, false);
            OnItemCountVisible.Broadcast(Index, PickUps[Index]->GetItemData().ItemType, false);
            PickUps[Index]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
            if (PickUps[Index]->Destroy())
            {
                PickUps[Index] = nullptr;
            }
        }
    }
}*/

/*void UFMBPlayerWeaponComponent::DestroyCurrentWeapon()
{
    OnItemSelected.Broadcast(CurrentWeaponIndex, CurrentWeapon->GetItemData().ItemType, false);
    OnItemIconVisibility.Broadcast(CurrentWeaponIndex, CurrentWeapon->GetItemData().ItemType, false);
    OnItemCountVisible.Broadcast(CurrentWeaponIndex, CurrentWeapon->GetItemData().ItemType, false);
    CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    if (CurrentWeapon->Destroy())
    {
        CurrentWeapon = nullptr;
    }
}

void UFMBPlayerWeaponComponent::DestroyCurrentPickUp()
{
    OnItemSelected.Broadcast(CurrentPickUpIndex, CurrentPickUp->GetItemData().ItemType, false);
    OnItemIconVisibility.Broadcast(CurrentPickUpIndex, CurrentPickUp->GetItemData().ItemType, false);
    OnItemCountVisible.Broadcast(CurrentPickUpIndex, CurrentPickUp->GetItemData().ItemType, false);
    CurrentPickUp->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    if (CurrentPickUp->Destroy())
    {
        CurrentPickUp = nullptr;
    }
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

void UFMBPlayerWeaponComponent::NextWeapon(bool bIsWheelDown)
{
    if (!CanEquip() || !CanAttack()) return;
    LastCurrentWeaponIndex = CurrentWeaponIndex;
    int8 CurrentWeaponIndexTemp{CurrentWeaponIndex};
    bIsWheelDown ? ++CurrentWeaponIndexTemp : --CurrentWeaponIndexTemp;
    if (CurrentWeaponIndexTemp > MaxWeapons - 1)
    {
        CurrentWeaponIndexTemp = 0;
    }
    if (CurrentWeaponIndexTemp < 0)
    {
        CurrentWeaponIndexTemp = MaxWeapons - 1;
    }
    CurrentWeaponIndex = CurrentWeaponIndexTemp;
    EquipNextWeapon();
}

void UFMBPlayerWeaponComponent::ChooseWeapon(int8 NewWeaponIndex)
{
    if (!CanEquip() || !CanAttack() || CurrentWeaponIndex == NewWeaponIndex) return;
    if (FMath::IsWithin<int8>(NewWeaponIndex, 0, MaxWeapons))
    {
        LastCurrentWeaponIndex = CurrentWeaponIndex;
        CurrentWeaponIndex = NewWeaponIndex;
        EquipNextWeapon();
    }
}

void UFMBPlayerWeaponComponent::EquipNextWeapon()
{
    if (!Character) return;

    bIsEquipAnimInProgress = true;
    PlayAnimMontage(CurrentWeaponAnimationsData.Equip);
}

void UFMBPlayerWeaponComponent::ThrowWeapon()
{
    if (!CanEquip() || !CanAttack()) return;
    ThrowItem(CurrentWeapon, CurrentWeaponIndex);
}

void UFMBPlayerWeaponComponent::ThrowPickUp()
{
    if (!CanEquip() || !CanAttack()) return;
    ThrowItem(CurrentPickUp, CurrentPickUpIndex);
}

void UFMBPlayerWeaponComponent::ThrowItem(AFMBBaseItem* ThrownItem, const int8 CurrentItemIndex)
{
    DropItem(ThrownItem, CurrentItemIndex);
    ClearCurrentItem(ThrownItem, CurrentItemIndex);
}

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

    if (CurrentPickUp->GetItemData().ItemCount < 1)
    {
        DestroyCurrentItem(CurrentPickUp, CurrentPickUpIndex);
    }
    // TestLogs();
}

void UFMBPlayerWeaponComponent::GetPickupItem(AFMBBaseItem* Item)
{
    if (!Item) return;
    SwapItem(Item);
}

void UFMBPlayerWeaponComponent::SwapItem(AFMBBaseItem* EquippedItem)
{
    if (!Character || !ItemInteractionComponent || !EquippedItem) return;

    const auto SwappingItem = [&](AFMBBaseItem* DroppedItem, const int8 DroppedItemIndex, const int8 NewIndex = 0)
    {
        if (NewIndex == INDEX_NONE)
        {
            DropItem(DroppedItem, DroppedItemIndex);
            ClearCurrentItem(DroppedItem, DroppedItemIndex);
        }
        ItemInteractionComponent->ClearHitItem();
    };
    if (EquippedItem->GetItemData().ItemType == EItemType::EIT_Weapon)
    {
        SwappingItem(CurrentWeapon, CurrentWeaponIndex, Weapons.Find(nullptr));
        EquipWeapon(Cast<AFMBBaseWeapon>(EquippedItem));
        return;
    }
    if (EquippedItem->GetItemData().ItemType == EItemType::EIT_PickUp)
    {
        const auto EquippedPickUp{Cast<AFMBBasePickUp>(EquippedItem)};
        SwappingItem(CurrentPickUp, CurrentPickUpIndex,
            CurrentPickUp && EquippedPickUp->GetPickUpType() != CurrentPickUp->GetPickUpType() ? INDEX_NONE : CurrentPickUpIndex);
        EquipPickUp(EquippedPickUp);
    }
}

void UFMBPlayerWeaponComponent::EquipWeapon(AFMBBaseWeapon* EquippedWeapon)
{
    if (!Character || !EquippedWeapon) return;

    EquippedWeapon->SetOwner(Character);
    if (CurrentWeapon)
    {
        const auto Index{Weapons.Find(nullptr)};
        if (Index != INDEX_NONE)
        {
            EquippingWeapon(EquippedWeapon, WeaponArmorySocketName, EItemState::EIS_PickedUp, Index, EquippedWeapon->GetItemData());
            Weapons[Index] = EquippedWeapon;
        }
    }
    else
    {
        CurrentWeaponAnimationsData = WeaponsAnimationsData.FindChecked(EquippedWeapon->GetWeaponType());
        CurrentWeapon = EquippedWeapon;
        Weapons[CurrentWeaponIndex] = EquippedWeapon;
        EquippingWeapon(EquippedWeapon, CurrentWeaponAnimationsData.WeaponEquipSocketName, EItemState::EIS_Equipped, CurrentWeaponIndex,
            EquippedWeapon->GetItemData());
        OnItemSelected.Broadcast(CurrentWeaponIndex, EquippedWeapon->GetItemData().ItemType, true);
    }
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickedUpSound, Character->GetActorLocation());
    // TestLogs();
}

void UFMBPlayerWeaponComponent::EquipPickUp(AFMBBasePickUp* EquippedPickUp)
{
    if (!EquippedPickUp || !Character) return;

    EquippedPickUp->SetOwner(Character);
    if (CurrentPickUp && EquippedPickUp->GetPickUpType() == CurrentPickUp->GetPickUpType())
    {
        EquippedPickUp->Destroy();
        CurrentPickUp->ChangeItemCount(true);
        OnItemCountChange.Broadcast(CurrentPickUpIndex, CurrentPickUp->GetItemData());
    }
    else
    {
        EquippingWeapon(EquippedPickUp, PickUpEquipSocketName, EItemState::EIS_Equipped, CurrentPickUpIndex, EquippedPickUp->GetItemData());
        CurrentPickUp = EquippedPickUp;
        OnItemSelected.Broadcast(CurrentPickUpIndex, EquippedPickUp->GetItemData().ItemType, true);
        OnItemCountChange.Broadcast(CurrentPickUpIndex, EquippedPickUp->GetItemData());
    }
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickedUpSound, Character->GetActorLocation());
    // TestLogs();
}

void UFMBPlayerWeaponComponent::EquippingWeapon(AFMBBaseItem* EquippedItem, const FName& WeaponSocketName, const EItemState NewItemState,
    const int32 NewIndex, const FItemData& NewItemData) const
{
    FMBUtils::AttachItemToSocket(EquippedItem, Character->GetMesh(), WeaponSocketName);
    EquippedItem->OnItemStateChanged.Broadcast(NewItemState);
    OnItemPickedUp.Broadcast(NewIndex, NewItemData);
};

void UFMBPlayerWeaponComponent::DropItem(AFMBBaseItem* DroppedItem, const int8 CurrentItemIndex) const
{
    Super::DropItem(DroppedItem, CurrentItemIndex);

    // if(!DroppedItem) return;
    OnItemIconVisibility.Broadcast(CurrentItemIndex, DroppedItem->GetItemData().ItemType, false);
    OnItemCountVisible.Broadcast(CurrentItemIndex, DroppedItem->GetItemData().ItemType, false);
    if (DroppedItem->GetItemData().ItemType == EItemType::EIT_PickUp)
    {
        OnItemSelected.Broadcast(CurrentItemIndex, DroppedItem->GetItemData().ItemType, false);
    }
}

/*int8 UFMBPlayerWeaponComponent::FindSimilarPickUpType(const AFMBBasePickUp* EquippedPickUp) const
{
    for (int8 Index = 0; Index < MaxPickUps; ++Index)
    {
        if (PickUps[Index] && PickUps[Index]->GetPickUpType() == EquippedPickUp->GetPickUpType())
        {
            return Index;
        }
    }
    return -1;
}*/

AFMBPlayerCharacter* UFMBPlayerWeaponComponent::GetPlayerCharacter() const
{
    return Cast<AFMBPlayerCharacter>(GetOwner());
}

void UFMBPlayerWeaponComponent::TestLogs()
{
    // UE_LOG(LogFMBPlayerWeaponComponent, Warning, TEXT("Weapons.Num = %d"), Weapons.Num());
    UE_LOG(LogFMBPlayerWeaponComponent, Warning, TEXT("CurrentWeaponIndex = %d"), CurrentWeaponIndex);
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
    if (CurrentPickUp)
    {
        UE_LOG(LogFMBPlayerWeaponComponent, Warning, TEXT("CurrentPickUp = %s"), *CurrentPickUp->GetName());
    }
    else
    {
        UE_LOG(LogFMBPlayerWeaponComponent, Warning, TEXT("CurrentPickUp == nullptr"));
    }
    /*for (const auto PickUp : PickUps)
    {
        if (PickUp)
        {
            UE_LOG(
                LogFMBPlayerWeaponComponent, Warning, TEXT("PickUp = %s, Count = %d"), *PickUp->GetName(),
                PickUp->GetItemData().ItemCount);
        }
        else
        {
            UE_LOG(LogFMBPlayerWeaponComponent, Warning, TEXT("PickUp == nullptr"));
        }
    }*/
}
