// For Maid Bilberry Game. All Rights Recerved

#include "Components/FMBStaminaComponent.h"
#include "Components/FMBWeaponComponent.h"
#include "FMBUtils.h"
#include "FMBCoreTypes.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBStaminaComponent, All, All)

UFMBStaminaComponent::UFMBStaminaComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UFMBStaminaComponent::BeginPlay()
{
    Super::BeginPlay();

    SetStamina(MaxStamina);
    check(MaxStamina > 0);

    StaminaSpends.Add(EStaminaSpend::ESS_Rolling, StaminaSpendData.RollingStaminaSpend);
    StaminaSpends.Add(EStaminaSpend::ESS_FastAttack, StaminaSpendData.FastAttackStaminaSpend);
    StaminaSpends.Add(EStaminaSpend::ESS_StrongAttack, StaminaSpendData.StrongAttackStaminaSpend);
}

bool UFMBStaminaComponent::CanSpendStamina(const EStaminaSpend StaminaSpend)
{
    if (!StaminaSpends.Contains(StaminaSpend)) return false;

    return FMath::IsWithinInclusive(Stamina - StaminaSpends[StaminaSpend], 0.0f, MaxStamina);
}

void UFMBStaminaComponent::SpendStamina(const EStaminaSpend StaminaSpend)
{
    StopHealStamina();

    SetStamina(GetStamina() - StaminaSpends[StaminaSpend]);
}

void UFMBStaminaComponent::SetStamina(float NewStamina)
{
    Stamina = FMath::Clamp(NewStamina, 0.0f, MaxStamina);
    OnStaminaChange.Broadcast(Stamina);
    // UE_LOG(LogFMBStaminaComponent, Display, TEXT("Stamina change: %0.0f"), Stamina);
}

void UFMBStaminaComponent::DecreaseRunningStamina()
{
    if (!GetWorld()) return;

    SetStamina(Stamina - StaminaModifier);

    if (IsStaminaZero() && GetWorld())
    {
        StopStaminaRunning();
    }
}

void UFMBStaminaComponent::AutoHealStamina()
{
    if (!GetWorld()) return;

    SetStamina(Stamina + StaminaModifier * StaminaHealModifier);

    if (IsStaminaFull() && GetWorld())
    {
        StopHealStamina();
    }
}

void UFMBStaminaComponent::StartHealStamina()
{
    if (!GetWorld()) return;
    if (GetWorld()->GetTimerManager().IsTimerActive(StaminaRunningTimerHandle)) return;

    if (!IsStaminaFull())
    {
        GetWorld()->GetTimerManager().SetTimer(
            StaminaAutoHealTimerHandle, this, &UFMBStaminaComponent::AutoHealStamina, StaminaUpdateTime, true, StaminaAutoHealDelay);
    }
}

void UFMBStaminaComponent::StopHealStamina()
{
    GetWorld()->GetTimerManager().ClearTimer(StaminaAutoHealTimerHandle);
}

void UFMBStaminaComponent::StartStaminaRunning()
{
    StopHealStamina();

    if (IsStaminaZero()) return;

    GetWorld()->GetTimerManager().SetTimer(
        StaminaRunningTimerHandle, this, &UFMBStaminaComponent::DecreaseRunningStamina, StaminaUpdateTime, true);
}

void UFMBStaminaComponent::StopStaminaRunning()
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