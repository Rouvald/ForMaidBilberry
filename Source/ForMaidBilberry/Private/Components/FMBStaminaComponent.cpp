// For Maid Bilberry Game. All Rights Reserved

#include "Components/FMBStaminaComponent.h"
#include "FMBCharacterMovementComponent.h"
#include "FMBCoreTypes.h"
#include "FMBHealthComponent.h"
#include "FMBPlayerCharacter.h"
#include "FMBUtils.h"

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

    PlayerCharacter = GetPlayerCharacter();
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

bool UFMBStaminaComponent::TryAddStamina(float StaminaAmount)
{
    if (IsStaminaFull() || !PlayerCharacter) return false;

    const auto HealthComponent{PlayerCharacter->FindComponentByClass<UFMBHealthComponent>()};
    if (!HealthComponent || HealthComponent->IsDead()) return false;

    SetStamina(Stamina + StaminaAmount);
    return true;
}

void UFMBStaminaComponent::SetStamina(float NewStamina)
{
    Stamina = FMath::Clamp(NewStamina, 0.0f, MaxStamina);
    OnStaminaChange.Broadcast(Stamina);
    // UE_LOG(LogFMBStaminaComponent, Display, TEXT("Stamina change: %0.0f"), Stamina);
}

void UFMBStaminaComponent::DecreaseRunningStamina()
{
    if (!GetWorld() || !PlayerCharacter) return;
    if (!PlayerCharacter->IsRunning())
    {
        StopStaminaRunning();
        return;
    }

    SetStamina(Stamina - StaminaModifier);

    if (IsStaminaZero() && GetWorld())
    {
        StopStaminaRunning();
    }
}

void UFMBStaminaComponent::AutoHealStamina()
{
    if (!PlayerCharacter || PlayerCharacter->IsRunning())
    {
        StartStaminaRunning();
        return;
    }

    SetStamina(Stamina + StaminaModifier * StaminaHealModifier);

    if (IsStaminaFull())
    {
        StopHealStamina();
    }
}

void UFMBStaminaComponent::StartHealStamina()
{
    if (!GetWorld() || GetWorld()->GetTimerManager().IsTimerActive(StaminaRunningTimerHandle)) return;

    if (!IsStaminaFull())
    {
        GetWorld()->GetTimerManager().SetTimer(
            StaminaAutoHealTimerHandle, this, &UFMBStaminaComponent::AutoHealStamina, StaminaUpdateTime, true, StaminaAutoHealDelay);
    }
}

void UFMBStaminaComponent::StopHealStamina()
{
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(StaminaAutoHealTimerHandle);
    }
}

void UFMBStaminaComponent::StartStaminaRunning()
{
    if (IsStaminaZero()) return;

    StopHealStamina();

    GetWorld()->GetTimerManager().SetTimer(
        StaminaRunningTimerHandle, this, &UFMBStaminaComponent::DecreaseRunningStamina, StaminaUpdateTime, true);
}

void UFMBStaminaComponent::StopStaminaRunning()
{
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(StaminaRunningTimerHandle);

        if (!GetWorld()->GetTimerManager().IsTimerActive(StaminaAutoHealTimerHandle))
        {
            StartHealStamina();
        }
    }
}

bool UFMBStaminaComponent::IsStaminaFull() const
{
    return FMath::IsNearlyEqual(Stamina, MaxStamina);
}

bool UFMBStaminaComponent::IsStaminaZero() const
{
    return FMath::IsNearlyZero(GetStamina());
}

AFMBPlayerCharacter* UFMBStaminaComponent::GetPlayerCharacter() const
{
    return Cast<AFMBPlayerCharacter>(GetOwner());
}
