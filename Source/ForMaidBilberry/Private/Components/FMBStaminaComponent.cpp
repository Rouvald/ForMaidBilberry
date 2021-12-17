// For Maid Bilberry Game. All Rights Recerved


#include "Components/FMBStaminaComponent.h"
#include "Components/FMBWeaponComponent.h"
#include "FMBUtils.h"
#include "FMBCoreTypes.h"

UFMBStaminaComponent::UFMBStaminaComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}


void UFMBStaminaComponent::BeginPlay()
{
    Super::BeginPlay();

    SetStamina(MaxStamina);
    check(MaxStamina > 0);

    StaminaSpends.Add(EStaminaSpend::Rolling, StaminaSpendData.RollingStaminaSpend);
    StaminaSpends.Add(EStaminaSpend::FastAttack, StaminaSpendData.FastAttackStaminaSpend);
    StaminaSpends.Add(EStaminaSpend::StrongAttack, StaminaSpendData.StrongAttackStaminaSpend);
}

bool UFMBStaminaComponent::SpendStamina(EStaminaSpend StaminaSpend)
{
    if (!StaminaSpends.Contains(StaminaSpend)) return false;

    if (!(FMath::IsWithin(GetStamina() - StaminaSpends[StaminaSpend], 0.0f, MaxStamina))) return false;

    StopHealStaminaTimer();

    SetStamina(GetStamina() - StaminaSpends[StaminaSpend]);

    return true;
}

void UFMBStaminaComponent::SetStamina(float NewStamina)
{
    Stamina = FMath::Clamp(NewStamina, 0.0f, MaxStamina);
    OnStaminaChange.Broadcast(Stamina);

    //UE_LOG(HealthLog, Display, TEXT("Stamina change: %0.0f"), GetStamina());
}

void UFMBStaminaComponent::DecreaseRunningStamina()
{
    if (!GetWorld()) return;

    SetStamina(Stamina - StaminaModifier);

    if (IsStaminaZero() && GetWorld())
    {
        StopStaminaRunningTimer();
    }
}

void UFMBStaminaComponent::AutoHealStamina()
{
    if (!GetWorld()) return;
    
    SetStamina(Stamina + StaminaModifier * StaminaHealModifier);

    if (IsStaminaFull() && GetWorld())
    {
        StopHealStaminaTimer();
    }
}

void UFMBStaminaComponent::StartHealStaminaTimer()
{
    if (!GetWorld()) return;
    if (GetWorld()->GetTimerManager().IsTimerActive(StaminaRunningTimerHandle)) return;

    if (!IsStaminaFull())
    {
        GetWorld()->GetTimerManager().SetTimer
            (
                StaminaAutoHealTimerHandle,
                this,
                &UFMBStaminaComponent::AutoHealStamina,
                StaminaUpdateTime,
                true,
                StaminaAutoHealDelay
                );
    }
}

void UFMBStaminaComponent::StopHealStaminaTimer()
{
    GetWorld()->GetTimerManager().ClearTimer(StaminaAutoHealTimerHandle);
}

void UFMBStaminaComponent::StartStaminaRunningTimer()
{
    StopHealStaminaTimer();

    if (IsStaminaZero()) return;

    GetWorld()->GetTimerManager().SetTimer
        (
            StaminaRunningTimerHandle,
            this,
            &UFMBStaminaComponent::DecreaseRunningStamina,
            StaminaUpdateTime,
            true
            );
}

void UFMBStaminaComponent::StopStaminaRunningTimer()
{
    GetWorld()->GetTimerManager().ClearTimer(StaminaRunningTimerHandle);
}

bool UFMBStaminaComponent::IsStaminaFull() const
{
    return FMath::IsNearlyEqual(Stamina, MaxStamina);
}

bool UFMBStaminaComponent::IsStaminaZero() const
{
    return FMath::IsNearlyZero(GetStamina());
}