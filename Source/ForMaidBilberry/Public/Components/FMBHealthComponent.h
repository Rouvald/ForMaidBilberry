// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FMBCoreTypes.h"
#include "FMBHealthComponent.generated.h"

class UCameraShakeBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORMAIDBILBERRY_API UFMBHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFMBHealthComponent();

    FOnDeathSignature OnDeath;
    FOnHealthChangeSignature OnHealthChange;

    UFUNCTION(BlueprintCallable, Category="Health")
    bool IsDead() const { return FMath::IsNearlyZero(Health); }

    UFUNCTION(BlueprintCallable, Category="Health")
    float GetHealthPercent() const { return Health / MaxHealth; }

    float GetHealth() const { return Health; }

    bool TryToAddHealth(float HealthAmount);
    bool IsHealthFull() const;

protected:

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

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
    TSubclassOf<UCameraShakeBase> CameraShake;

    virtual void BeginPlay() override;
    
private:
    float Health = 0.0f;
    FTimerHandle HealTimeHandle;

    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy,
        AActor* DamageCauser);

    void AutoHealing();
    void SetHealth(float NewHealth);

    bool CheckAllAnimInProgress() const;

    void PlayCameraShake() const;
    void PlayHitAnimation() const;

    void Killed(AController* KillerController);
};
