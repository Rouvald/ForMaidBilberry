// For Maid Bilberry Game. All Rights Recerved


#include "UI/FMBPlayerHUDWidget.h"
#include "Components/FMBHealthComponent.h"
#include "Components/FMBWeaponComponent.h"
#include "FMBUtils.h"

bool UFMBPlayerHUDWidget::Initialize()
{
    const auto HealthComponent = FMBUtils::GetFMBPlayerComponent<UFMBHealthComponent>(GetOwningPlayerPawn());
    if (HealthComponent)
    {
        HealthComponent->OnHealthChange.AddUObject(this, &UFMBPlayerHUDWidget::OnHealthChange);
    }
    return Super::Initialize();
}

void UFMBPlayerHUDWidget::OnHealthChange(float Health, float HealthDelta)
{
    if (HealthDelta < 0.0f)
    {
        OnTakeDamage();
    }
}

float UFMBPlayerHUDWidget::GetHealthPercent() const
{
    const auto HealthComponent = FMBUtils::GetFMBPlayerComponent<UFMBHealthComponent>(GetOwningPlayerPawn());
    if (!HealthComponent) return 0.0f;

    return HealthComponent->GetHealthPercent();
}

float UFMBPlayerHUDWidget::GetStaminaPercent() const
{
    const auto HealthComponent = FMBUtils::GetFMBPlayerComponent<UFMBHealthComponent>(GetOwningPlayerPawn());
    if (!HealthComponent) return 0.0f;

    return HealthComponent->GetStaminaPercent();
}

bool UFMBPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& WeaponUIData) const
{
    const auto WeaponComponent = FMBUtils::GetFMBPlayerComponent<UFMBWeaponComponent>(GetOwningPlayerPawn());
    if (!WeaponComponent) return false;

    //WeaponUIData.WeaponName = CheckWeaponName(WeaponUIData);

    return WeaponComponent->GetCurrentWeaponUIData(WeaponUIData);
}

bool UFMBPlayerHUDWidget::GetArmoryWeaponUIData(FWeaponUIData& WeaponUIData) const
{
    const auto WeaponComponent = FMBUtils::GetFMBPlayerComponent<UFMBWeaponComponent>(GetOwningPlayerPawn());
    if (!WeaponComponent) return false;

    //WeaponUIData.WeaponName = CheckWeaponName(WeaponUIData);

    return WeaponComponent->GetArmoryWeaponUIData(WeaponUIData);
}

bool UFMBPlayerHUDWidget::IsPlayerAlive() const
{
    const auto HealthComponent = FMBUtils::GetFMBPlayerComponent<UFMBHealthComponent>(GetOwningPlayerPawn());

    return HealthComponent && !HealthComponent->IsDead();
}

bool UFMBPlayerHUDWidget::IsPlayerSpectating() const
{
    const auto Controller = GetOwningPlayer();
    return Controller && Controller->GetStateName() == NAME_Spectating;
}