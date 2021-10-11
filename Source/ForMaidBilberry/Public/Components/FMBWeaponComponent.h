// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
//#include "FMBBaseCharacter.h"
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

    //FOnStaminaChangeSignature OnStaminaChange;

    void FastMeleeAttack();
    void StrongMeleeAttack();

    void StopDrawTrace() const;

    bool GetAttackAnimInProgress() const { return AttackAnimInProgress;}

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

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    UPROPERTY()
    AFMBBaseWeapon* CurrentWeapon = nullptr;
    bool AttackAnimInProgress = false;

    void SpawnWeapon();
    void InitAnimation(UAnimMontage* Animation);

    UFUNCTION()
    void OnAttackFinished(USkeletalMeshComponent* MeshComp);

    bool CanAttack() const;

    void StartMovement();
    void StopMovement();

    AFMBBaseCharacter* GetCharacter();
    UCharacterMovementComponent* GetMovementComponent();

    //void DecreaseStaminaAttack(float DecreaseStamina);

    void AttachWeaponToSocket(AFMBBaseWeapon* Weapon, USkeletalMeshComponent* MeshComp, const FName& WeaponSocket);
};
