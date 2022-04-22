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
    FOnItemPickedUpSignature OnItemPickedUp;
    FOnItemIconVisiblitySignature OnItemIconVisibility;
    FOnItemSelectedSignature OnItemSelected;
    FOnItemCountChangeSignature OnItemCountChange;
    FOnItemCountVisibilitySignature OnItemCountVisible;

    // void OnStartBlock();
    // void OnStopBlock();

    void UsePickUp();
    void PickUpWasUsed();

    void GetPickupItem(AFMBBaseItem* Item);

    FORCEINLINE bool GetIsBlocking() const { return bIsShieldBlocking; }

    FORCEINLINE int8 GetMaxWeapons() const { return MaxWeapons; }
    FORCEINLINE int8 GetMaxPickUps() const { return MaxPickUps; }

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
    virtual void InitWeaponComponent() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    // virtual void EquipItems() override;

    virtual void EquipWeapon(AFMBBaseWeapon* EquippedWeapon) override;

private:
    UPROPERTY()
    UFMBItemInteractionComponent* ItemInteractionComponent{nullptr};

    UPROPERTY()
    TArray<AFMBBaseWeapon*> Weapons;

    UPROPERTY()
    TArray<AFMBBasePickUp*> PickUps;

    /* Max Weapons that player can hold. */
    int8 MaxWeapons{4};
    /* Max PickUps that player can hold. */
    int8 MaxPickUps{2};

    int8 CurrentWeaponIndex{0};
    int8 CurrentPickUpIndex{0};

    /* Taken Items */
    /*UPROPERTY()
    AFMBBaseShield* CurrentShield{nullptr};*/

    UPROPERTY()
    AFMBBasePickUp* CurrentPickUp{nullptr};
    //

    bool bIsShieldBlocking{false};

    // void SpawnEquipShields();

    void EquipPickUp(AFMBBasePickUp* EquippedPickUp);
    void SwapPickUp(AFMBBasePickUp* EquippedPickUp);

    void SwapWeapon(AFMBBaseWeapon* EquippedWeapon);

    void DropItem(AFMBBaseItem* DropItem) const;

    void ClearCurrentWeapon();

    // void DestroyCurrentShield();
    void DestroyPickUps();
    void DestroyWeapons();

    AFMBPlayerCharacter* GetPlayerCharacter() const;

    void TestLogs();
};
