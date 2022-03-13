// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/FMBBaseWeaponComponent.h"
#include "FMBPlayerWeaponComponent.generated.h"

class AFMBBaseShield;
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

    FORCEINLINE bool GetIsBlocking() const { return bIsShieldBlocking; }

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<AFMBBaseShield> ShieldClass;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TMap<EWeaponType, FName> WeaponShieldMaps;

    virtual void BeginPlay() override;

    void PlayAnimMontage(UAnimMontage* Animation) const;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    virtual void SpawnItems() override;

private:
    UPROPERTY()
    AFMBBaseShield* CurrentShield{nullptr};

    bool bIsShieldBlocking{false};

    void SpawnShields();
};
