// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FMBCoreTypes.h"
#include "FMBWeaponComponent.generated.h"

class AFMBBaseCharacter;
class UFMBCharacterMovementComponent;
class UFMBStaminaComponent;
class AFMBBaseWeapon;
class AFMBBaseShield;
class UAnimMontage;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FORMAIDBILBERRY_API UFMBWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFMBWeaponComponent();

    void FastMeleeAttack();
    void StrongMeleeAttack();

    void OnStartBlock();
    void OnStopBlock();

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

    FORCEINLINE bool GetIsBlocking() const { return bIsBlocking; }

protected:
    // UPROPERTY(EditDefaultsOnly, Category="Shield")
    // UStaticMeshComponent* Shield;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TArray<TSubclassOf<AFMBBaseWeapon>> WeaponClasses;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<AFMBBaseShield> ShieldClass;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TMap<EWeaponType, FName> WeaponShieldMaps;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponArmorySocketName = "WeaponArmorySocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animations")
    TMap<EWeaponType, FWeaponAnimationsData> WeaponsAnimationsData;

    virtual void BeginPlay() override;

    void PlayAnimMontage(UAnimMontage* Animation) const;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    // int32 GetCurrentWeaponIndex() const {return CurrentWeaponIndex;}

private:
    //
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
    AFMBBaseShield* CurrentShield{nullptr};

    UPROPERTY()
    AFMBBaseWeapon* ArmoryWeapon{nullptr};

    UPROPERTY()
    FWeaponAnimationsData CurrentWeaponAnimationsData;

    bool AttackAnimInProgress{false};

    bool EquipAnimInProgress{false};

    UPROPERTY()
    TArray<AFMBBaseWeapon*> Weapons;

    /*UPROPERTY()
    TArray<AFMBBaseShield*> Shields;*/

    bool bIsBlocking{false};
    int32 CurrentWeaponIndex{0};

    void SpawnItems();
    void SpawnWeapons();
    void SpawnShields();
    void EquipWeapon();

    void CheckWeaponAnimationsData();
    void InitAnimation(const FWeaponAnimationsData& WeaponAnimationData);
    void CheckAttackAnimNotifyState(UAnimMontage* Animation);

    void OnAttackNotifyStateBegin(USkeletalMeshComponent* MeshComp);
    void OnAttackNotifyStateEnd(USkeletalMeshComponent* MeshComp);
    void OnAttackNotifyAnimEnd(USkeletalMeshComponent* MeshComp);
    void OnEquipFinished(USkeletalMeshComponent* MeshComp);
    void OnChangeEquipWeapon(USkeletalMeshComponent* MeshComp);
    void EquipShield();

    bool CanDoAttack(const EStaminaSpend AttackStaminaSpend) const;
    void SpendStamina(const EStaminaSpend StaminaSpend) const;
};
