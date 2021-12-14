// For Maid Bilberry Game. All Rights Recerved


#include "UI/FMBPlayerHUDWidget.h"
#include "Components/FMBHealthComponent.h"
#include "Components/FMBStaminaComponent.h"
#include "Components/FMBWeaponComponent.h"
#include "FMBUtils.h"

void UFMBPlayerHUDWidget::NativeOnInitialized()
{
    if(GetOwningPlayer())
    {
        GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &UFMBPlayerHUDWidget::OnNewPawn);
        OnNewPawn(GetOwningPlayerPawn());
    }
}

void UFMBPlayerHUDWidget::OnNewPawn(APawn* NewPawn)
{
    const auto HealthComponent = FMBUtils::GetFMBPlayerComponent<UFMBHealthComponent>(NewPawn);
    if (HealthComponent)
    {
        HealthComponent->OnHealthChange.AddUObject(this, &UFMBPlayerHUDWidget::OnHealthChange);
    }
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
    const auto StaminaComponent = FMBUtils::GetFMBPlayerComponent<UFMBStaminaComponent>(GetOwningPlayerPawn());
    if (!StaminaComponent) return 0.0f;

    return StaminaComponent->GetStaminaPercent();
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
