// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
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

    void StopDrawTrace() const;

    //bool GetAttackAnimInProgress() const { return AttackAnimInProgress;}
    //bool GetEquipAnimInProgress() const { return EquipAnimInProgress;}

    bool CanAttack() const;
    bool CanEquip() const;

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
    
    void PlayAnimMontage(UAnimMontage* Animation);

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    //int32 GetCurrentWeaponIndex() const {return CurrentWeaponIndex;}

private:
    UPROPERTY()
    AFMBBaseWeapon* CurrentWeapon = nullptr;
    
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
    
    void StartMovement();
    void StopMovement();

    AFMBBaseCharacter* GetCharacter() const;
    UCharacterMovementComponent* GetMovementComponent() const;
};
