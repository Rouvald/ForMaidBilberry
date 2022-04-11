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

    PlayerCharacter = GetPlayerCharacter();
    if (PlayerCharacter)
    {
        ItemInteractionComponent = PlayerCharacter->FindComponentByClass<UFMBItemInteractionComponent>();
    }
}

void UFMBPlayerWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    CurrentShield->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    CurrentShield->Destroy();

    Super::EndPlay(EndPlayReason);
}

void UFMBPlayerWeaponComponent::SpawnItems()
{
    Super::SpawnItems();
    SpawnShields();
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
    FMBUtils::AttachItemToSocket(CurrentShield, Character->GetMesh(), TEXT("WeaponEquipSocket_L"));
}

void UFMBPlayerWeaponComponent::OnStartBlock()
{
    if (!CurrentShield) return;

    // CurrentShield->Startblocking();
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

void UFMBPlayerWeaponComponent::EquipPickUp(AFMBBasePickUp* EquippedPickUp)
{
    if (!EquippedPickUp) return;
    if (!PlayerCharacter) return;

    EquippedPickUp->SetOwner(PlayerCharacter);
    /*if (WeaponDataMap.Contains(EquippedWeapon->GetWeaponType()))
    {
        CurrentWeaponData = WeaponDataMap[EquippedWeapon->GetWeaponType()];
    }*/

    FMBUtils::AttachItemToSocket(EquippedPickUp, Character->GetMesh(), PickUpEquipSocketName);

    CurrentPickUp = EquippedPickUp;
    CurrentPickUp->OnItemStateChanged.Broadcast(EItemState::EIS_Equipped);
}

void UFMBPlayerWeaponComponent::EquipWeapon(AFMBBaseWeapon* EquippedWeapon)
{
    if (!EquippedWeapon) return;
    if (!PlayerCharacter) return;

    EquippedWeapon->SetOwner(PlayerCharacter);
    if (WeaponsAnimationsData.Contains(EquippedWeapon->GetWeaponType()))
    {
        CurrentWeaponAnimationsData = WeaponsAnimationsData[EquippedWeapon->GetWeaponType()];
    }

    FMBUtils::AttachItemToSocket(EquippedWeapon, Character->GetMesh(), CurrentWeaponAnimationsData.WeaponEquipSocketName);

    CurrentWeapon = EquippedWeapon;
    CurrentWeapon->OnItemStateChanged.Broadcast(EItemState::EIS_Equipped);
    // CurrentWeapon->GetItemMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    // CurrentWeapon->GetItemMesh()->SetSimulatePhysics(false);
}

void UFMBPlayerWeaponComponent::SwapPickUp(AFMBBasePickUp* EquippedPickUp)
{
    if (!PlayerCharacter && !ItemInteractionComponent) return;

    DropItem(CurrentPickUp);
    EquipPickUp(EquippedPickUp);
    ItemInteractionComponent->ClearHitItem();
}

void UFMBPlayerWeaponComponent::SwapWeapon(AFMBBaseWeapon* EquippedWeapon)
{
    if (!PlayerCharacter && !ItemInteractionComponent) return;

    DropItem(CurrentWeapon);
    EquipWeapon(EquippedWeapon);
    ItemInteractionComponent->ClearHitItem();
}

void UFMBPlayerWeaponComponent::DropItem(const AFMBBaseItem* DropItem) const
{
    if (!DropItem) return;

    const auto ItemMesh = DropItem->GetItemMesh();
    if (!ItemMesh) return;

    const FDetachmentTransformRules DetachmentTransformRules{EDetachmentRule::KeepWorld, true};
    ItemMesh->DetachFromComponent(DetachmentTransformRules);

    DropItem->OnItemStateChanged.Broadcast(EItemState::EIS_Falling);
    // DropItem->ThrowWeapon();
}

AFMBPlayerCharacter* UFMBPlayerWeaponComponent::GetPlayerCharacter() const
{
    return Cast<AFMBPlayerCharacter>(GetOwner());
}