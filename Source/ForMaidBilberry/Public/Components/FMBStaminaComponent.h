// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FMBCoreTypes.h"
#include "FMBStaminaComponent.generated.h"

class AFMBPlayerCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FORMAIDBILBERRY_API UFMBStaminaComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFMBStaminaComponent();

    FOnStaminaChangeSignature OnStaminaChange;

    FORCEINLINE float GetStaminaPercent() const { return Stamina / MaxStamina; }
    FORCEINLINE float GetStamina() const { return Stamina; }

    bool CanSpendStamina(const EStaminaSpend StaminaSpend);
    void SpendStamina(const EStaminaSpend StaminaSpend);

    void StartHealStamina();
    void StopHealStamina();

    void StartStaminaRunning();
    void StopStaminaRunning();

    bool TryAddStamina(float StaminaAmount);

    FORCEINLINE FStaminaSpendData GetStaminaSpendData() const { return StaminaSpendData; }

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina", meta = (ClampMin = "0.0", ClampMax = "2000.0"))
    float MaxStamina{100.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina")
    float StaminaModifier{1.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina")
    float StaminaHealModifier{1.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina")
    float StaminaUpdateTime{0.1f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina")
    float StaminaAutoHealDelay{1.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina")
    FStaminaSpendData StaminaSpendData;

    virtual void BeginPlay() override;

private:
    UPROPERTY()
    AFMBPlayerCharacter* PlayerCharacter{nullptr};

    FTimerHandle StaminaRunningTimerHandle;
    FTimerHandle StaminaAutoHealTimerHandle;
    float Stamina{0.0f};

    TMap<EStaminaSpend, float> StaminaSpends;

    void SetStamina(float NewStamina);

    void DecreaseRunningStamina();
    void AutoHealStamina();

    bool IsStaminaFull() const;
    bool IsStaminaZero() const;

    AFMBPlayerCharacter* GetPlayerCharacter() const;
};
