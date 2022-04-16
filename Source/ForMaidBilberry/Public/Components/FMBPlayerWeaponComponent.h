// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/FMBBaseWeaponComponent.h"
#include "FMBCoreTypes.h"
#include "FMBPlayerWeaponComponent.generated.h"

class AFMBPlayerCharacter;
class AFMBBaseShield;
class AFMBBasePickUp;
class UFMBItemInteractionComponent;
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBPlayerWeaponComponent : public UFMBBaseWeaponComponent
{
    GENERATED_BODY()
public:
    FOnPickUpPickedUpSignature OnPickUpPickedUp;
    FOnPickUpCountChangeSignature OnPickUpCountChange;

    void OnStartBlock();
    void OnStopBlock();

    void GetPickupItem(AFMBBaseItem* Item);

    FORCEINLINE bool GetIsBlocking() const { return bIsShieldBlocking; }

    FORCEINLINE int8 GetMaxWeapons() const { return MaxWeapons; }

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<AFMBBaseShield> ShieldClass;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TMap<EWeaponType, FName> WeaponShieldMaps;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName ShieldEquipSocketName{"WeaponEquipSocket_L"};

    UPROPERTY(EditDefaultsOnly, Category = "PickUp")
    FName PickUpEquipSocketName{"PickUpEquipSocket"};

    virtual void BeginPlay() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    virtual void SpawnItems() override;

    virtual void EquipWeapon(AFMBBaseWeapon* EquippedWeapon) override;

private:
    UPROPERTY()
    UFMBItemInteractionComponent* ItemInteractionComponent{nullptr};

    UPROPERTY()
    TArray<AFMBBaseWeapon*> Weapons;

    /* Max Weapons that player can hold. */
    int8 MaxWeapons{5};

    int8 CurrentWeaponIndex{0};

    /* Taken Items */
    UPROPERTY()
    AFMBBaseShield* CurrentShield{nullptr};

    UPROPERTY()
    AFMBBasePickUp* CurrentPickUp{nullptr};
    //

    bool bIsShieldBlocking{false};

    void SpawnShields();

    void EquipPickUp(AFMBBasePickUp* EquippedPickUp);
    void SwapPickUp(AFMBBasePickUp* EquippedPickUp);

    void SwapWeapon(AFMBBaseWeapon* EquippedWeapon);

    void DropItem(AFMBBaseItem* DropItem) const;

    void ClearCurrentWeapon();

    void DestroyCurrentShield() const;
    void DestroyCurrentPickUp() const;
    void DestroyWeapons();

    AFMBPlayerCharacter* GetPlayerCharacter() const;
};
