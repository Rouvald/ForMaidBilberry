// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "FMBBaseWeapon.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "FMBCoreTypes.h"
#include "FMBBaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UFMBHealthComponent;
class UTextRenderComponent;
class UFMBWeaponComponent;

UCLASS()
class FORMAIDBILBERRY_API AFMBBaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    FOnStaminaChangeSignature OnStaminaChange;

    //FName FPPCameraSocketName = "FPPCameraSocket";

    AFMBBaseCharacter(const FObjectInitializer& ObjInit);

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable, Category="Movement")
    bool IsRunning() const;

    float GetStamina() const { return Stamina; }

    UFUNCTION(BlueprintCallable, Category="Movement")
    float GetMovementDirection() const;

protected:
    UPROPERTY(EditDefaultsOnly, Category="Backpack")
    UStaticMeshComponent* Backpack;

    UPROPERTY(EditDefaultsOnly, Category="Weapon")
    FName BackpackSocketName = "Backpack";

    //UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    //float BaseTurnAroundRate = 45.0f;

    //UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    //float BaseLookUpRate = 45.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    UCameraComponent* TPPCameraComponent;

    //UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    //UCameraComponent* FPPCameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    UFMBHealthComponent* HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    UTextRenderComponent* HealthTextComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    UFMBWeaponComponent* WeaponComponent;

    UPROPERTY(EditDefaultsOnly, Category="Animation")
    UAnimMontage* DeathAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category="Damage")
    float LifeSpanOnDeath = 5.0f;

    UPROPERTY(EditDefaultsOnly, Category="Damage")
    FVector2D LandedVelocityZ = {900.f, 1200.0f};

    UPROPERTY(EditDefaultsOnly, Category="Damage")
    FVector2D LandedDamage = {10.f, 100.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Stamina", meta=(ClampMin="0.0", ClampMax="200.0"))
    float MaxStamina = 100.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Stamina")
    float StaminaModifier = 3.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Stamina")
    float StaminaUpdateTime = 0.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Stamina")
    float StaminaAutoHealDelay = 5.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Spend Stamina")
    float JumpStaminaSpend = 10.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Spend Stamina")
    float FastAttackStaminaSpend = 30.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Spend Stamina")
    float StrongAttackStaminaSpend = 50.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Spend Stamina")
    float RollingStaminaSpend = 15.0f;

    virtual void BeginPlay() override;

private:
    bool WantToRun = false;
    //bool IsMovingForward = false;

    FTimerHandle StaminaRunningTimerHandle;
    FTimerHandle StaminaAutoHealTimerHandle;
    float Stamina = 0.0f;

    //bool JumpAnimInProgress = false;

    //void SwitchCamera();

    void MoveForward(float Amount);
    void MoveRight(float Amount);

    virtual void Jump() override;
    //bool CanJump() const;

    //void LimitViewPitchRotation ();
    //void TurnAroundAtRate(float Rate);
    //void LookUpAtRate(float Rate);

    void OnStartRunning();
    void OnStopRunning();

    void OnDeath();
    void OnHealthChange(float Health);

    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hitresult);

    void FastMeleeAttack();
    void StrongMeleeAttack();

    void SetStamina(float NewStamina);

    bool SpendStamina(float SpendStaminaVal);

    void DecreaseRunningStamina();
    void SetAutoHealStaminaTimer();
    void CheckActiveHealStaminaTimer();
    void AutoHealStamina();

    void Rolling();

    bool CheckAllAnimInProgress() const;
};
