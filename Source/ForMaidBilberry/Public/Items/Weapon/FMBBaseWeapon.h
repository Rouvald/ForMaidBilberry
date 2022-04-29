// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "FMBCoreTypes.h"
#include "GameFramework/Actor.h"
#include "Items/FMBBaseItem.h"
#include "FMBBaseWeapon.generated.h"

class AFMBBaseItem;
class UStaticMeshComponent;
class UFMBWeaponFXComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class USoundCue;

UCLASS()
class FORMAIDBILBERRY_API AFMBBaseWeapon : public AFMBBaseItem
{
    GENERATED_BODY()

public:
    AFMBBaseWeapon();

    virtual void MeleeAttack(const EChooseAttack ChooseAttack);

    // UFUNCTION(BlueprintCallable)
    // FWeaponAnimationData GetWeaponAnimationData() const {return WeaponAnimationData;}

    // UFUNCTION(BlueprintCallable)
    FORCEINLINE const EWeaponType& GetWeaponType() const { return WeaponType; }

    virtual void StopDrawTrace();

    FORCEINLINE const FWeaponDamageData& GetWeaponDamageData() const { return WeaponDamageData; }

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
    UFMBWeaponFXComponent* WeaponFXComponent{nullptr};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    EWeaponType WeaponType{EWeaponType::EWT_SwordShield};

    UPROPERTY()
    float DamageAmount{0};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FWeaponDamageData WeaponDamageData;

    TMap<EChooseAttack, float> ChooseDamageAmount;

    virtual void BeginPlay() override;

    virtual void SetItemInfo() const override;
};
