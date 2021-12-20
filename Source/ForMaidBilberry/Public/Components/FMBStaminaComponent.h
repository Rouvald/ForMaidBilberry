// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FMBCoreTypes.h"
#include "FMBStaminaComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FORMAIDBILBERRY_API UFMBStaminaComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFMBStaminaComponent();

    FOnStaminaChangeSignature OnStaminaChange;

    UFUNCTION(BlueprintCallable, Category = "Stamina")
    float GetStaminaPercent() const { return Stamina / MaxStamina; }

    float GetStamina() const { return Stamina; }

    bool SpendStamina(EStaminaSpend StaminaSpend);

    void StartHealStaminaTimer();
    void StopHealStaminaTimer();

    void StartStaminaRunningTimer();
    void StopStaminaRunningTimer();

    FStaminaSpendData GetStaminaSpendData() const { return StaminaSpendData; }

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina", meta = (ClampMin = "0.0", ClampMax = "2000.0"))
    float MaxStamina = 100.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina")
    float StaminaHealModifier = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina")
    float StaminaModifier = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina")
    float StaminaUpdateTime = 0.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina")
    float StaminaAutoHealDelay = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina")
    FStaminaSpendData StaminaSpendData;

    virtual void BeginPlay() override;

private:
    FTimerHandle StaminaRunningTimerHandle;
    FTimerHandle StaminaAutoHealTimerHandle;
    float Stamina = 0.0f;

    TMap<EStaminaSpend, float> StaminaSpends;

    void SetStamina(float NewStamina);

    void DecreaseRunningStamina();
    void AutoHealStamina();

    bool IsStaminaFull() const;
    bool IsStaminaZero() const;
};
