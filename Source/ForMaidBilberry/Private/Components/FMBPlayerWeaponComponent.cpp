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

    InitWeaponComponent();

    CurrentWeaponIndex = 0;
    CurrentPickUpIndex = 0;

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

void UFMBPlayerWeaponComponent::OnChangeEquipWeapon(USkeletalMeshComponent* MeshComp)
{
    if (CurrentWeaponIndex < 0 || CurrentWeaponIndex > Weapons.Num())
    {
        UE_LOG(LogFMBPlayerWeaponComponent, Warning, TEXT("Incorrect Weapon Index"));
        return;
    }
    if (!Character || Character->GetMesh() != MeshComp) return;

    if (CurrentWeapon)
    {
        CurrentWeapon->StopDrawTrace();
        CurrentWeapon->OnItemStateChanged.Broadcast(EItemState::EIS_PickedUp);
        FMBUtils::AttachItemToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
    }
    OnItemSelected.Broadcast(LastCurrentWeaponIndex, EItemType::EIT_Weapon, false);
    CurrentWeapon = Weapons[CurrentWeaponIndex];
    if (CurrentWeapon)
    {
        CurrentWeapon->OnItemStateChanged.Broadcast(EItemState::EIS_Equipped);
        if (WeaponsAnimationsData.Contains(CurrentWeapon->GetWeaponType()))
        {
            CurrentWeaponAnimationsData = WeaponsAnimationsData[CurrentWeapon->GetWeaponType()];
        }
        FMBUtils::AttachItemToSocket(CurrentWeapon, Character->GetMesh(), CurrentWeaponAnimationsData.WeaponEquipSocketName);
        OnItemSelected.Broadcast(CurrentWeaponIndex, CurrentWeapon->GetItemData().ItemType, true);
    }
    else
    {
        if (WeaponsAnimationsData.Contains(EWeaponType::EWT_NoWeapon))
        {
            CurrentWeaponAnimationsData = WeaponsAnimationsData[EWeaponType::EWT_NoWeapon];
        }
        OnItemSelected.Broadcast(CurrentWeaponIndex, EItemType::EIT_Weapon, true);
    }
}

void UFMBPlayerWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp)
{
    if (!Character || Character->GetMesh() != MeshComp) return;
    bIsEquipAnimInProgress = false;
}

void UFMBPlayerWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    DestroyWeapons();
    // DestroyCurrentShield();
    DestroyPickUps();

    // TestLogs();
}

void UFMBPlayerWeaponComponent::DestroyWeapons()
{
    if (CurrentWeapon)
    {
        OnItemSelected.Broadcast(CurrentWeaponIndex, CurrentWeapon->GetItemData().ItemType, false);
        OnItemSelected.Broadcast(0, EItemType::EIT_Weapon, true);
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

void UFMBPlayerWeaponComponent::DestroyCurrentWeapon()
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
    if (FMath::IsWithin(NewWeaponIndex, static_cast<int8>(0), MaxWeapons))
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
    DropItem(CurrentWeapon);
    ClearCurrentWeapon();
}

void UFMBPlayerWeaponComponent::ThrowPickUp()
{
    if (!CanEquip() || !CanAttack()) return;
    DropItem(CurrentPickUp);
    ClearCurrentPickUp();
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
        DestroyCurrentPickUp();
    }
    // TestLogs();
}

void UFMBPlayerWeaponComponent::GetPickupItem(AFMBBaseItem* Item)
{
    if (!Item) return;

    switch (Item->GetItemData().ItemType)
    {
    case EItemType::EIT_Weapon:
    {
        const auto Weapon = Cast<AFMBBaseWeapon>(Item);
        if (Weapon)
        {
            SwapWeapon(Weapon);
        }
        break;
    }
    case EItemType::EIT_PickUp:
    {
        const auto PickUp = Cast<AFMBBasePickUp>(Item);
        if (PickUp)
        {
            SwapPickUp(PickUp);
        }
        break;
    }
    default:
        break;
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
        OnItemSelected.Broadcast(CurrentWeaponIndex, EquippedWeapon->GetItemData().ItemType, true);
    }
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickedUpSound, Character->GetActorLocation());
    // TestLogs();
}

void UFMBPlayerWeaponComponent::EquipPickUp(AFMBBasePickUp* EquippedPickUp)
{
    if (!EquippedPickUp || !Character) return;

    if (CurrentPickUp)
    {
        const auto SimilarPickUpTypeIndex{FindSimilarPickUpType(EquippedPickUp)};
        if (SimilarPickUpTypeIndex > -1)
        {
            EquippedPickUp->Destroy();
            PickUps[SimilarPickUpTypeIndex]->ChangeItemCount(true);
            OnItemCountChange.Broadcast(SimilarPickUpTypeIndex, PickUps[SimilarPickUpTypeIndex]->GetItemData());
        }
        else
        {
            for (int8 Index = 0; Index < MaxPickUps; ++Index)
            {
                if (!PickUps[Index])
                {
                    PickUps[Index] = EquippedPickUp;
                    EquippedPickUp->SetOwner(Character);
                    FMBUtils::AttachItemToSocket(EquippedPickUp, Character->GetMesh(), WeaponArmorySocketName);
                    EquippedPickUp->OnItemStateChanged.Broadcast(EItemState::EIS_PickedUp);
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
        PickUps[CurrentPickUpIndex] = EquippedPickUp;
        EquippedPickUp->OnItemStateChanged.Broadcast(EItemState::EIS_Equipped);
        OnItemPickedUp.Broadcast(CurrentPickUpIndex, EquippedPickUp->GetItemData());
        OnItemSelected.Broadcast(CurrentPickUpIndex, EquippedPickUp->GetItemData().ItemType, true);
        OnItemCountChange.Broadcast(CurrentPickUpIndex, EquippedPickUp->GetItemData());
    }
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickedUpSound, Character->GetActorLocation());
    // TestLogs();
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

void UFMBPlayerWeaponComponent::SwapPickUp(AFMBBasePickUp* EquippedPickUp)
{
    if (!Character || !ItemInteractionComponent) return;

    if (PickUps.Find(nullptr) == INDEX_NONE)
    {
        if (FindSimilarPickUpType(EquippedPickUp) == -1)
        {
            DropItem(CurrentPickUp);
            ClearCurrentPickUp();
        }
    }
    EquipPickUp(EquippedPickUp);
    ItemInteractionComponent->ClearHitItem();
}

void UFMBPlayerWeaponComponent::ClearCurrentWeapon()
{
    CurrentWeapon = nullptr;
    Weapons[CurrentWeaponIndex] = nullptr;
}

void UFMBPlayerWeaponComponent::ClearCurrentPickUp()
{
    CurrentPickUp = nullptr;
    PickUps[CurrentPickUpIndex] = nullptr;
}

void UFMBPlayerWeaponComponent::DropItem(AFMBBaseItem* DropItem) const
{
    if (!DropItem) return;

    const auto ItemMesh = DropItem->GetItemMesh();
    if (!ItemMesh) return;

    switch (DropItem->GetItemData().ItemType)
    {
    case EItemType::EIT_Weapon:
    {
        OnItemIconVisibility.Broadcast(CurrentWeaponIndex, CurrentWeapon->GetItemData().ItemType, false);
        break;
    }
    case EItemType::EIT_PickUp:
    {
        OnItemSelected.Broadcast(CurrentPickUpIndex, CurrentPickUp->GetItemData().ItemType, false);
        OnItemIconVisibility.Broadcast(CurrentPickUpIndex, CurrentPickUp->GetItemData().ItemType, false);
        OnItemCountVisible.Broadcast(CurrentPickUpIndex, CurrentPickUp->GetItemData().ItemType, false);
        break;
    }
    default:
        break;
    }
    const FDetachmentTransformRules DetachmentTransformRules{EDetachmentRule::KeepWorld, true};
    ItemMesh->DetachFromComponent(DetachmentTransformRules);

    DropItem->OnItemStateChanged.Broadcast(EItemState::EIS_Falling);
    DropItem->ThrowWeapon();
}

int8 UFMBPlayerWeaponComponent::FindSimilarPickUpType(const AFMBBasePickUp* EquippedPickUp) const
{
    for (int8 Index = 0; Index < MaxPickUps; ++Index)
    {
        if (PickUps[Index] && PickUps[Index]->GetPickUpType() == EquippedPickUp->GetPickUpType())
        {
            return Index;
        }
    }
    return -1;
}

AFMBPlayerCharacter* UFMBPlayerWeaponComponent::GetPlayerCharacter() const
{
    return Cast<AFMBPlayerCharacter>(GetOwner());
}

void UFMBPlayerWeaponComponent::TestLogs()
{
    // UE_LOG(LogFMBPlayerWeaponComponent, Warning, TEXT("Weapons.Num = %d"), Weapons.Num());
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
    for (const auto PickUp : PickUps)
    {
        if (PickUp)
        {
            UE_LOG(
                LogFMBPlayerWeaponComponent, Warning, TEXT("PickUp = %s, Count = %d"), *PickUp->GetName(), PickUp->GetItemData().ItemCount);
        }
        else
        {
            UE_LOG(LogFMBPlayerWeaponComponent, Warning, TEXT("PickUp == nullptr"));
        }
    }
}
