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
    void OnStartBlock();
    void OnStopBlock();

    void GetPickupItem(AFMBBaseItem* Item);

    FORCEINLINE bool GetIsBlocking() const { return bIsShieldBlocking; }

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<AFMBBaseShield> ShieldClass;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TMap<EWeaponType, FName> WeaponShieldMaps;

    UPROPERTY(EditDefaultsOnly, Category = "PickUp")
    FName PickUpEquipSocketName = "PickUpEquipSocket";

    virtual void BeginPlay() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    virtual void SpawnItems() override;

private:
    UPROPERTY()
    AFMBPlayerCharacter* PlayerCharacter{nullptr};

    UPROPERTY()
    UFMBItemInteractionComponent* ItemInteractionComponent{nullptr};

    /* Taken Items */
    UPROPERTY()
    AFMBBaseShield* CurrentShield{nullptr};

    UPROPERTY()
    TArray<const AFMBBaseWeapon*> Weapons;

    UPROPERTY()
    AFMBBasePickUp* CurrentPickUp{nullptr};
    //

    bool bIsShieldBlocking{false};

    void SpawnShields();

    void EquipPickUp(AFMBBasePickUp* EquippedPickUp);
    void SwapPickUp(AFMBBasePickUp* EquippedPickUp);

    void EquipWeapon(AFMBBaseWeapon* EquippedWeapon);
    void SwapWeapon(AFMBBaseWeapon* EquippedWeapon);

    void DropItem(const AFMBBaseItem* DropItem) const;

    AFMBPlayerCharacter* GetPlayerCharacter() const;
};
