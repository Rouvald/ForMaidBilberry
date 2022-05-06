// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FMBCoreTypes.h"
#include "FMBBaseWeaponComponent.generated.h"

class AFMBBaseCharacter;
class UFMBCharacterMovementComponent;
class UFMBStaminaComponent;
class AFMBBaseWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FORMAIDBILBERRY_API UFMBBaseWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFMBBaseWeaponComponent();

    void MeleeAttack(EStaminaSpend StaminaSpendType);
    void StopDrawTrace();

    FORCEINLINE bool CanAttack() const { return !bIsAttackAnimInProgress; };
    FORCEINLINE bool CanEquip() const { return !bIsEquipAnimInProgress; }

    FORCEINLINE AFMBBaseWeapon* GetCurrentWeapon() const { return CurrentWeapon; }
    FORCEINLINE FWeaponAnimationsData GetCurrentWeaponAnimationsData() const { return CurrentWeaponAnimationsData; }
    FORCEINLINE const TMap<EWeaponType, FWeaponAnimationsData>& GetWeaponAnimationsData() const { return WeaponsAnimationsData; }

protected:
    UPROPERTY()
    AFMBBaseCharacter* Character{nullptr};

    UPROPERTY()
    UFMBCharacterMovementComponent* MovementComponent{nullptr};

    UPROPERTY()
    UFMBStaminaComponent* StaminaComponent{nullptr};
    //

    UPROPERTY()
    AFMBBaseWeapon* CurrentWeapon{nullptr};

    UPROPERTY()
    FWeaponAnimationsData CurrentWeaponAnimationsData;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponArmorySocketName = "WeaponArmorySocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animations")
    TMap<EWeaponType, FWeaponAnimationsData> WeaponsAnimationsData;

    bool bIsEquipAnimInProgress{false};

    virtual void BeginPlay() override;

    void InitWeaponComponent();

    void PlayAnimMontage(UAnimMontage* Animation) const;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    virtual void EquipItems();

    AFMBBaseWeapon* SpawnWeapon() const;
    virtual void EquipWeapon(AFMBBaseWeapon* EquippedWeapon);

    virtual void DropItems();
    virtual void DropItem(AFMBBaseItem* DroppedItem, const int8 CurrentItemIndex = 0) const;

    virtual void InitAnimation(const FWeaponAnimationsData& WeaponAnimationData);

private:
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<AFMBBaseWeapon> WeaponClass;

    bool bIsAttackAnimInProgress{false};
    EChooseAttack CurrentAttackType{EChooseAttack::ECA_Max};

    void CheckWeaponAnimationsData();
    void CheckAttackAnimNotifyState(UAnimMontage* Animation);

    void OnAttackNotifyStateBegin(USkeletalMeshComponent* MeshComp);
    void OnAttackNotifyStateEnd(USkeletalMeshComponent* MeshComp);
    void OnAttackNotifyAnimEnd(USkeletalMeshComponent* MeshComp);

    bool CanDoAttack(const EStaminaSpend AttackStaminaSpend) const;
    void SpendStamina(const EStaminaSpend StaminaSpend) const;
};
