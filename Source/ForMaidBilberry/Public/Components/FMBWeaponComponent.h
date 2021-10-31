// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "FMBBaseWeapon.h"
#include "FMBCharacterMovementComponent.h"
#include "Components/ActorComponent.h"
#include "FMBWeaponComponent.generated.h"

class AFMBBaseCharacter;
class AFMBBaseWeapon;
class UAnimMontage;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORMAIDBILBERRY_API UFMBWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFMBWeaponComponent();

    void FastMeleeAttack();
    void StrongMeleeAttack();

    void NextWeapon();

    void StopDrawTrace();

    //bool GetAttackAnimInProgress() const { return AttackAnimInProgress;}
    //bool GetEquipAnimInProgress() const { return EquipAnimInProgress;}

    bool CanAttack() const;
    bool CanEquip() const;

    bool GetCurrentWeaponUIData(FWeaponUIData& WeaponUIData) const;
    bool GetArmoryWeaponUIData(FWeaponUIData& WeaponUIData) const;

    AFMBBaseWeapon* GetCurrentWeapon() const { return CurrentWeapon; }

protected:
    UPROPERTY(EditDefaultsOnly, Category="Weapon")
    TArray<TSubclassOf<AFMBBaseWeapon>> WeaponClasses;

    UPROPERTY(EditDefaultsOnly, Category="Weapon")
    FName WeaponEquipSocketName = "WeaponEquipSocket_L"; // LeftWeaponShield

    UPROPERTY(EditDefaultsOnly, Category="Weapon")
    FName WeaponArmorySocketName = "WeaponArmorySocket";

    UPROPERTY(EditDefaultsOnly, Category="Animation")
    UAnimMontage* FastMeleeAttackAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category="Animation")
    UAnimMontage* StrongMeleeAttackAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category="Animation")
    UAnimMontage* EquipAnimMontage;

    virtual void BeginPlay() override;

    void PlayAnimMontage(UAnimMontage* Animation) const;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    //int32 GetCurrentWeaponIndex() const {return CurrentWeaponIndex;}

private:
    int32 FastAttackStaminaSpend = 0;
    int32 StrongAttackStaminaSpend = 1;

    UPROPERTY()
    AFMBBaseWeapon* CurrentWeapon = nullptr;

    UPROPERTY()
    AFMBBaseWeapon* ArmoryWeapon = nullptr;

    bool AttackAnimInProgress = false;

    bool EquipAnimInProgress = false;

    UPROPERTY()
    TArray<AFMBBaseWeapon*> Weapons;

    int32 CurrentWeaponIndex = 0;

    void SpawnWeapons();
    void AttachWeaponToSocket(AFMBBaseWeapon* Weapon, USceneComponent* MeshComp, const FName& WeaponSocket);
    void EquipWeapon(int32 WeaponIndex);

    void InitAnimation();
    void CheckAttackFinishedAnimNotify(UAnimMontage* Animation);

    void OnAttackFinished(USkeletalMeshComponent* MeshComp);
    void OnEquipFinished(USkeletalMeshComponent* MeshComp);
    void OnChangeEquipWeapon(USkeletalMeshComponent* MeshComp);

    void StartMovement() const;
    void StopMovement() const;

    AFMBBaseCharacter* GetCharacter() const;
};
