// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FMBCoreTypes.h"
#include "FMBHealthComponent.generated.h"

class UAnimMontage;
class UCameraShakeBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORMAIDBILBERRY_API UFMBHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFMBHealthComponent();

    FOnDeathSignature OnDeath;
    FOnHealthChangeSignature OnHealthChange;

    FOnStaminaChangeSignature OnStaminaChange;

    UFUNCTION(BlueprintCallable, Category="Health")
    bool IsDead() const { return FMath::IsNearlyZero(Health); }

    UFUNCTION(BlueprintCallable, Category="Health")
    float GetHealthPercent() const { return Health / MaxHealth; }

    UFUNCTION(BlueprintCallable, Category="Stamina")
    float GetStaminaPercent() const { return Stamina / MaxStamina; }

    float GetHealth() const { return Health; }

    bool TryToAddHealth(float HealthAmount);
    bool IsHealthFull() const;

    float GetStamina() const { return Stamina; }

    bool SpendStamina(EStaminaSpend StaminaSpend);

    void StartHealStaminaTimer();
    void CheckAndStopHealStaminaTimer();

    void StartStaminaRunningTimer();
    void CheckAndStopStaminaRunningTimer();

    FStaminaSpendData GetStaminaSpendData () const {return StaminaSpendData;}

protected:
    //UPROPERTY(EditDefaultsOnly, Category="Animation")
    //UAnimMontage* GetHitAnimMontage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Health", meta=(ClampMin="0.0", ClampMax="500.0"))
    float MaxHealth = 100.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Health")
    bool AutoHeal = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Health", meta = (EditCondition = "AutoHeal"))
    float HealUpdateTime = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Health", meta = (EditCondition = "AutoHeal"))
    float HealDelay = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Health", meta = (EditCondition = "AutoHeal"))
    float HealModifier = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Stamina", meta=(ClampMin="0.0", ClampMax="2000.0"))
    float MaxStamina = 100.0f;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Stamina")
    float StaminaModifier = 3.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Stamina")
    float StaminaUpdateTime = 0.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Stamina")
    float StaminaAutoHealDelay = 5.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Stamina")
    FStaminaSpendData StaminaSpendData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
    TSubclassOf<UCameraShakeBase> CameraShake;

    virtual void BeginPlay() override;
private:
    float Health = 0.0f;
    FTimerHandle HealTimeHandle;

    FTimerHandle StaminaRunningTimerHandle;
    FTimerHandle StaminaAutoHealTimerHandle;
    float Stamina = 0.0f;

    TMap<EStaminaSpend, float> StaminaSpends;

    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy,
        AActor* DamageCauser);

    void AutoHealing();
    void SetHealth(float NewHealth);

    bool CheckAllAnimInProgress() const;

    void SetStamina(float NewStamina);

    void DecreaseRunningStamina();
    void AutoHealStamina();

    float ChooseSpendStamina(int32 SpendStaminaValue) const;

    void PlayCameraShake() const;

    void Killed(AController* KillerController);
};
