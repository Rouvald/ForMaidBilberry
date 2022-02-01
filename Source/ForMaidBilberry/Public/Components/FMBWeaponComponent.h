// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "FMBBaseWeapon.h"
#include "FMBCharacterMovementComponent.h"
#include "Components/ActorComponent.h"
#include "FMBCoreTypes.h"
#include "FMBWeaponComponent.generated.h"

class UStaticMeshComponent;
class AFMBBaseCharacter;
class AFMBBaseWeapon;
class UAnimMontage;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FORMAIDBILBERRY_API UFMBWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFMBWeaponComponent();

    void FastMeleeAttack();
    void StrongMeleeAttack();

    void NextWeapon();

    void StopDrawTrace();

    // bool GetAttackAnimInProgress() const { return AttackAnimInProgress;}
    // bool GetEquipAnimInProgress() const { return EquipAnimInProgress;}

    bool CanAttack() const;
    bool CanEquip() const;

    bool GetCurrentWeaponUIData(FWeaponUIData& WeaponUIData) const;
    bool GetArmoryWeaponUIData(FWeaponUIData& WeaponUIData) const;

    /*UFUNCTION(BlueprintCallable)
    const TArray<AFMBBaseWeapon*>& GetWeapons() const { return Weapons.Num() ? nullptr : Weapons; }*/

    // const TArray<TSubclassOf<AFMBBaseWeapon>>& GetWeaponClasses() const { return WeaponClasses; }

    FORCEINLINE AFMBBaseWeapon* GetCurrentWeapon() const { return CurrentWeapon; }
    FORCEINLINE FWeaponAnimationsData GetCurrentWeaponAnimationsData() const { return CurrentWeaponAnimationsData; }
    FORCEINLINE int32 GetCurrentWeaponIndex() const { return CurrentWeaponIndex; }

    FORCEINLINE const TMap<EWeaponType, FWeaponAnimationsData>& GetWeaponAnimationsData() const { return WeaponsAnimationsData; }

protected:
    // UPROPERTY(EditDefaultsOnly, Category="Shield")
    // UStaticMeshComponent* Shield;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TArray<TSubclassOf<AFMBBaseWeapon>> WeaponClasses;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponArmorySocketName = "WeaponArmorySocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animations")
    TMap<EWeaponType, FWeaponAnimationsData> WeaponsAnimationsData;

    virtual void BeginPlay() override;

    void PlayAnimMontage(UAnimMontage* Animation) const;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    // int32 GetCurrentWeaponIndex() const {return CurrentWeaponIndex;}

private:
    UPROPERTY()
    AFMBBaseCharacter* Character{nullptr};

    UPROPERTY()
    AFMBBaseWeapon* CurrentWeapon{nullptr};

    UPROPERTY()
    AFMBBaseWeapon* ArmoryWeapon{nullptr};

    UPROPERTY()
    FWeaponAnimationsData CurrentWeaponAnimationsData;

    bool AttackAnimInProgress{false};

    bool EquipAnimInProgress{false};

    UPROPERTY()
    TArray<AFMBBaseWeapon*> Weapons;

    int32 CurrentWeaponIndex{0};

    bool IsPlayerCharacter{false};

    void SpawnWeapons();
    void AttachWeaponToSocket(AFMBBaseWeapon* Weapon, USceneComponent* MeshComp, const FName& WeaponSocket) const;
    void EquipWeapon(int32 WeaponIndex);

    void CheckWeaponAnimationsData();
    void InitAnimation(const FWeaponAnimationsData& WeaponAnimationData);
    void CheckAttackFinishedAnimNotify(UAnimMontage* Animation);

    void OnAttackFinished(USkeletalMeshComponent* MeshComp);
    void OnEquipFinished(USkeletalMeshComponent* MeshComp);
    void OnChangeEquipWeapon(USkeletalMeshComponent* MeshComp);

    // void ShieldVisibility(EWeaponType CurrentWeaponType) const;

    bool CanDoAttack(const EStaminaSpend AttackStaminaSpend) const;
    void SpendStamina(const EStaminaSpend StaminaSpend) const;
};
