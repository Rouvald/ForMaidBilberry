// For Maid Bilberry Game. All Rights Recerved


#include "Components/FMBHealthComponent.h"

#include "FMBBaseCharacter.h"

DECLARE_LOG_CATEGORY_CLASS(HealthLog, All, All);

UFMBHealthComponent::UFMBHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}


void UFMBHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    SetHealth(MaxHealth);

    const auto Controller = GetOwner();
    if (Controller)
    {
        Controller->OnTakeAnyDamage.AddDynamic(this, &UFMBHealthComponent::OnTakeAnyDamage);
    }
}

void UFMBHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
    class AController* InstigatedBy,
    AActor* DamageCauser)
{
    if (Damage <= 0.0f || IsDead() && GetWorld()) return;

    SetHealth(Health - Damage);

    const auto DamagedCharacter = Cast<AFMBBaseCharacter>(DamagedActor);
    if (DamagedCharacter)
    {
        DamagedCharacter->PlayAnimMontage(GetHitAnimMontage);
    }

    GetWorld()->GetTimerManager().ClearTimer(HealTimeHandle);

    UE_LOG(HealthLog, Display, TEXT("Health: %f"), GetHealth());

    if (IsDead())
    {
        OnDeath.Broadcast();
    }
    else if (AutoHeal)
    {
        GetWorld()->GetTimerManager().SetTimer(HealTimeHandle, this, &UFMBHealthComponent::AutoHealing, HealUpdateTime, true, HealDelay);
    }
}

void UFMBHealthComponent::AutoHealing()
{
    SetHealth(Health + HealModifier);

    if (FMath::IsNearlyEqual(Health, MaxHealth) && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(HealTimeHandle);
    }
}

void UFMBHealthComponent::SetHealth(float NewHealth)
{
    Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    OnHealthChange.Broadcast(Health);
}
