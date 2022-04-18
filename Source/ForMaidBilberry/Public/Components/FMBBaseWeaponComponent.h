// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FMBCoreTypes.h"
#include "FMBBaseWeaponComponent.generated.h"

class AFMBBaseCharacter;
class UFMBCharacterMovementComponent;
class UFMBStaminaComponent;
class AFMBBaseWeapon;
class UAnimMontage;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FORMAIDBILBERRY_API UFMBBaseWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFMBBaseWeaponComponent();

    void FastMeleeAttack();
    void StrongMeleeAttack();

    // void NextWeapon();

    void StopDrawTrace();

    // bool GetAttackAnimInProgress() const { return bIsAttackAnimInProgress;}
    // bool GetEquipAnimInProgress() const { return bIsEquipAnimInProgress;}

    bool CanAttack() const;
    bool CanEquip() const;

    // UTexture2D* GetCurrentWeaponUIImage() const;
    // bool GetArmoryWeaponUIData(FItemData& WeaponUIData) const;

    /*UFUNCTION(BlueprintCallable)
    const TArray<AFMBBaseWeapon*>& GetWeapons() const { return Weapons.Num() ? nullptr : Weapons; }*/

    // const TArray<TSubclassOf<AFMBBaseWeapon>>& GetWeaponClasses() const { return WeaponClasses; }

    FORCEINLINE AFMBBaseWeapon* GetCurrentWeapon() const { return CurrentWeapon; }
    FORCEINLINE FWeaponAnimationsData GetCurrentWeaponAnimationsData() const { return CurrentWeaponAnimationsData; }
    // FORCEINLINE int32 GetCurrentWeaponIndex() const { return CurrentWeaponIndex; }

    FORCEINLINE const TMap<EWeaponType, FWeaponAnimationsData>& GetWeaponAnimationsData() const { return WeaponsAnimationsData; }

protected:
    // UPROPERTY(EditDefaultsOnly, Category="Shield")
    // UStaticMeshComponent* Shield;

    /*UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TArray<TSubclassOf<AFMBBaseWeapon>> WeaponClasses;*/
    //
    UPROPERTY()
    AFMBBaseCharacter* Character{nullptr};

    UPROPERTY()
    UFMBCharacterMovementComponent* MovementComponent{nullptr};

    UPROPERTY()
    UFMBStaminaComponent* StaminaComponent{nullptr};
    //

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<AFMBBaseWeapon> WeaponClass;

    UPROPERTY()
    AFMBBaseWeapon* CurrentWeapon{nullptr};

    /*UPROPERTY()
    AFMBBaseWeapon* ArmoryWeapon{nullptr};*/

    UPROPERTY()
    FWeaponAnimationsData CurrentWeaponAnimationsData;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponArmorySocketName = "WeaponArmorySocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animations")
    TMap<EWeaponType, FWeaponAnimationsData> WeaponsAnimationsData;

    virtual void BeginPlay() override;

    void PlayAnimMontage(UAnimMontage* Animation) const;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    // int32 GetCurrentWeaponIndex() const {return CurrentWeaponIndex;}

    virtual void EquipItems();

    AFMBBaseWeapon* SpawnWeapon() const;
    virtual void EquipWeapon(AFMBBaseWeapon* EquippedWeapon);

private:
    bool bIsAttackAnimInProgress{false};
    bool bIsEquipAnimInProgress{false};

    /*UPROPERTY()
    TArray<AFMBBaseWeapon*> Weapons;*/

    /*UPROPERTY()
    TArray<AFMBBaseShield*> Shields;*/

    // int32 CurrentWeaponIndex{0};

    // void EquipWeapon();

    void CheckWeaponAnimationsData();
    void InitAnimation(const FWeaponAnimationsData& WeaponAnimationData);
    void CheckAttackAnimNotifyState(UAnimMontage* Animation);

    void OnAttackNotifyStateBegin(USkeletalMeshComponent* MeshComp);
    void OnAttackNotifyStateEnd(USkeletalMeshComponent* MeshComp);
    void OnAttackNotifyAnimEnd(USkeletalMeshComponent* MeshComp);
    // void OnEquipFinished(USkeletalMeshComponent* MeshComp);
    // void OnChangeEquipWeapon(USkeletalMeshComponent* MeshComp);
    // void EquipShield();

    bool CanDoAttack(const EStaminaSpend AttackStaminaSpend) const;
    void SpendStamina(const EStaminaSpend StaminaSpend) const;
};
