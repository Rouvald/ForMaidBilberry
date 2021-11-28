// For Maid Bilberry Game. All Rights Recerved


#include "Components/FMBHealthComponent.h"
#include "FMBBaseCharacter.h"
#include "Components/FMBCharacterMovementComponent.h"
#include "Components/FMBWeaponComponent.h"
#include "FMBUtils.h"
#include "FMBCoreTypes.h"

DECLARE_LOG_CATEGORY_CLASS(HealthLog, All, All);

UFMBHealthComponent::UFMBHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}


void UFMBHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    check(MaxHealth > 0);
    check(MaxStamina > 0);

    SetHealth(MaxHealth);
    SetStamina(MaxStamina);

    const auto Controller = GetOwner();
    if (Controller)
    {
        Controller->OnTakeAnyDamage.AddDynamic(this, &UFMBHealthComponent::OnTakeAnyDamage);
    }

    StaminaSpends.Add(EStaminaSpend::Rolling, StaminaSpendData.RollingStaminaSpend);
    StaminaSpends.Add(EStaminaSpend::FastAttack, StaminaSpendData.FastAttackStaminaSpend);
    StaminaSpends.Add(EStaminaSpend::StrongAttack, StaminaSpendData.StrongAttackStaminaSpend);
}

void UFMBHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
    class AController* InstigatedBy,
    AActor* DamageCauser)
{
    if (Damage <= 0.0f || IsDead() && GetWorld()) return;

    // Get Hit , if stay in damage sphere anim don't loop // NEED FIX!!
    if (CheckAllAnimInProgress())
    {
        if (const auto Character = Cast<AFMBBaseCharacter>(GetOwner()))
        {
            const auto WeaponComponent = FMBUtils::GetFMBPlayerComponent<UFMBWeaponComponent>(Character);
            if (WeaponComponent || WeaponComponent->GetCurrentWeapon())
            {
                Character->PlayAnimMontage(WeaponComponent->GetCurrentWeaponAnimationsData().GetHit);
            }
            //==================================================
            //UE_LOG(HealthLog, Display, TEXT("GetHit Animation play"),);
            //==================================================
        }
    }
    //

    SetHealth(Health - Damage);

    GetWorld()->GetTimerManager().ClearTimer(HealTimeHandle);

    //UE_LOG(HealthLog, Display, TEXT("Health: %f"), GetHealth());

    if (IsDead())
    {
        OnDeath.Broadcast();
    }
    else if (AutoHeal)
    {
        GetWorld()->GetTimerManager().SetTimer(HealTimeHandle, this, &UFMBHealthComponent::AutoHealing, HealUpdateTime, true, HealDelay);
    }
    PlayCameraShake();
}

void UFMBHealthComponent::AutoHealing()
{
    SetHealth(Health + HealModifier);

    if (IsHealthFull() && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(HealTimeHandle);
    }
}

void UFMBHealthComponent::SetHealth(float NewHealth)
{
    const auto NextHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    const auto HealthDelta = NextHealth - Health;

    Health = NextHealth;
    OnHealthChange.Broadcast(Health, HealthDelta);
}

bool UFMBHealthComponent::TryToAddHealth(float HealthAmount)
{
    if (IsDead() || IsHealthFull()) return false;

    SetHealth(Health + HealthAmount);
    return true;
}

bool UFMBHealthComponent::IsHealthFull() const
{
    return FMath::IsNearlyEqual(Health, MaxHealth);
}

bool UFMBHealthComponent::CheckAllAnimInProgress() const
{
    const auto MovementComponent = FMBUtils::GetFMBPlayerComponent<UFMBCharacterMovementComponent>(GetOwner());
    if (!MovementComponent || !(MovementComponent->CanRolling())) return false;

    const auto WeaponComponent = FMBUtils::GetFMBPlayerComponent<UFMBWeaponComponent>(GetOwner());
    if (!WeaponComponent || !(WeaponComponent->CanAttack()) || !(WeaponComponent->CanEquip())) return false;
    return true;
}

bool UFMBHealthComponent::SpendStamina(EStaminaSpend StaminaSpend)
{
    if (!StaminaSpends.Contains(StaminaSpend)) return false;

    if (!(FMath::IsWithin(GetStamina() - StaminaSpends[StaminaSpend], 0.0f, MaxStamina))) return false;

    CheckAndStopHealStaminaTimer();

    SetStamina(GetStamina() - StaminaSpends[StaminaSpend]);

    return true;
}

void UFMBHealthComponent::SetStamina(float NewStamina)
{
    Stamina = FMath::Clamp(NewStamina, 0.0f, MaxStamina);
    OnStaminaChange.Broadcast(Stamina);

    //UE_LOG(HealthLog, Display, TEXT("Stamina change: %0.0f"), GetStamina());
}

void UFMBHealthComponent::DecreaseRunningStamina()
{
    if (!GetWorld()) return;
    //const auto MovementComponent = FMBUtils::GetFMBPlayerComponent<UFMBCharacterMovementComponent>(GetOwner());
    //if(!MovementComponent || MovementComponent->IsFalling()) return;

    const auto WeaponComponent = FMBUtils::GetFMBPlayerComponent<UFMBWeaponComponent>(GetOwner());
    if (!WeaponComponent || !(WeaponComponent->CanAttack())) return;

    if (GetStamina() <= 0.0f)
    {
        CheckAndStopStaminaRunningTimer();
    }
    SetStamina(Stamina - StaminaModifier);
}

void UFMBHealthComponent::AutoHealStamina()
{
    SetStamina(Stamina + StaminaModifier);

    if (FMath::IsNearlyEqual(Stamina, MaxStamina) && GetWorld())
    {
        CheckAndStopHealStaminaTimer();
    }
}

void UFMBHealthComponent::StartHealStaminaTimer()
{
    if (!GetWorld()) return;
    if (GetWorld()->GetTimerManager().IsTimerActive(StaminaRunningTimerHandle))
    {
        return;
    }

    CheckAndStopHealStaminaTimer();

    if (FMath::IsWithin(GetStamina(), 0.0f, MaxStamina))
    {
        GetWorld()->GetTimerManager().SetTimer
            (
                StaminaAutoHealTimerHandle,
                this,
                &UFMBHealthComponent::AutoHealStamina,
                StaminaUpdateTime,
                true,
                StaminaAutoHealDelay
                );
    }
}

void UFMBHealthComponent::CheckAndStopHealStaminaTimer()
{
    if (GetWorld()->GetTimerManager().IsTimerActive(StaminaAutoHealTimerHandle))
    {
        GetWorld()->GetTimerManager().ClearTimer(StaminaAutoHealTimerHandle);
    }
}

void UFMBHealthComponent::StartStaminaRunningTimer()
{
    CheckAndStopHealStaminaTimer();

    if (FMath::IsNearlyZero(GetStamina())) return;

    GetWorld()->GetTimerManager().SetTimer
        (
            StaminaRunningTimerHandle,
            this,
            &UFMBHealthComponent::DecreaseRunningStamina,
            StaminaUpdateTime,
            true
            );
}

void UFMBHealthComponent::CheckAndStopStaminaRunningTimer()
{
    if (GetWorld()->GetTimerManager().IsTimerActive(StaminaRunningTimerHandle))
    {
        GetWorld()->GetTimerManager().ClearTimer(StaminaRunningTimerHandle);
    }
}

void UFMBHealthComponent::PlayCameraShake() const
{
    if (IsDead()) return;
    const auto Player = Cast<APawn>(GetOwner());
    if (!Player) return;

    const auto PlayerController = Player->GetController<APlayerController>();
    if (!PlayerController || !PlayerController->PlayerCameraManager) return;

    PlayerController->PlayerCameraManager->StartCameraShake(CameraShake);
}
