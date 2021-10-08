// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "FMBCharacterMovementComponent.h"
#include "Components/ActorComponent.h"
#include "FMBWeaponComponent.generated.h"

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

protected:
    UPROPERTY(EditDefaultsOnly, Category="Weapon")
    TSubclassOf<AFMBBaseWeapon> WeaponClass;

    UPROPERTY(EditDefaultsOnly, Category="Weapon")
    FName WeaponSocketName = "LeftWeaponShield";

    UPROPERTY(EditDefaultsOnly, Category="Animation")
    UAnimMontage* FastMeleeAttackAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category="Animation")
    UAnimMontage* StrongMeleeAttackAnimMontage;

    virtual void BeginPlay() override;
    void PlayAnimMontage(UAnimMontage* Animation);

private:
    UPROPERTY()
    AFMBBaseWeapon* CurrentWeapon = nullptr;
    bool AttackAnimInProgress = false;

    void SpawnWeapon();
    void InitAnimation(UAnimMontage* Animation);
    void OnAttackFinished(USkeletalMeshComponent* MeshComp);

    bool CanAttack() const;

    void StartMovement();
    void StopMovement();
    UCharacterMovementComponent* GetMovementComponent();
};
