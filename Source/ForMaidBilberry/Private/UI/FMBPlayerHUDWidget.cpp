// For Maid Bilberry Game. All Rights Recerved


#include "UI/FMBPlayerHUDWidget.h"
#include "Components/FMBHealthComponent.h"
#include "Components/FMBWeaponComponent.h"

float UFMBPlayerHUDWidget::GetHealthPercent() const
{
    const auto HealthComponent = GetHealthComponent();

    return HealthComponent->GetHealthPercent();
 }

float UFMBPlayerHUDWidget::GetStaminaPercent() const
{
    const auto HealthComponent = GetHealthComponent();
    if(!HealthComponent) return 0.0f;

    return HealthComponent->GetStaminaPercent();
}

UFMBHealthComponent* UFMBPlayerHUDWidget::GetHealthComponent() const
{
    const auto Player = GetOwningPlayerPawn();
    if(!Player) return nullptr;

    const auto Component = Player->GetComponentByClass(UFMBHealthComponent::StaticClass());
    const auto HealthComponent = Cast<UFMBHealthComponent>(Component);
    if (!HealthComponent) return nullptr;

    return HealthComponent;
}

bool UFMBPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& WeaponUIData) const
{
    const auto Player = GetOwningPlayerPawn();
    if(!Player) return false;

    const auto Component = Player->GetComponentByClass(UFMBWeaponComponent::StaticClass());
    const auto WeaponComponent = Cast<UFMBWeaponComponent>(Component);
    if (!WeaponComponent) return false;

    return WeaponComponent->GetCurrentWeaponUIData(WeaponUIData);
}

bool UFMBPlayerHUDWidget::GetArmoryWeaponUIData(FWeaponUIData& WeaponUIData) const
{
    const auto Player = GetOwningPlayerPawn();
    if(!Player) return false;

    const auto Component = Player->GetComponentByClass(UFMBWeaponComponent::StaticClass());
    const auto WeaponComponent = Cast<UFMBWeaponComponent>(Component);
    if (!WeaponComponent) return false;

    return WeaponComponent->GetArmoryWeaponUIData(WeaponUIData);
}