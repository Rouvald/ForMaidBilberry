// For Maid Bilberry Game. All Rights Reserved

#include "UI/FMBPlayerHUDWidget.h"

#include "FMBPlayerCharacter.h"
#include "Components/FMBHealthComponent.h"
#include "Components/FMBStaminaComponent.h"
#include "Components/FMBBaseWeaponComponent.h"
#include "FMBUtils.h"
#include "FMBWeaponDataWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBPlayerHUDWidget, All, All)

void UFMBPlayerHUDWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (GetOwningPlayer())
    {
        GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &UFMBPlayerHUDWidget::OnNewPawn);
        OnNewPawn(GetOwningPlayerPawn());
    }
}

void UFMBPlayerHUDWidget::OnNewPawn(APawn* NewPawn)
{
    const auto HealthComponent{FMBUtils::GetFMBPlayerComponent<UFMBHealthComponent>(NewPawn)};
    if (HealthComponent)
    {
        HealthComponent->OnHealthChange.AddUObject(this, &UFMBPlayerHUDWidget::OnHealthChange);
    }
}

void UFMBPlayerHUDWidget::OnHealthChange(float Health, float HealthDelta)
{
    if (HealthDelta < 0.0f)
    {
        // OnTakeDamage();
        if (!IsAnimationPlaying(BloodyScreenAnim))
        {
            PlayAnimation(BloodyScreenAnim);
        }
    }
}

float UFMBPlayerHUDWidget::GetHealthPercent() const
{
    const auto HealthComponent{FMBUtils::GetFMBPlayerComponent<UFMBHealthComponent>(GetOwningPlayerPawn())};
    if (!HealthComponent) return 0.0f;

    return HealthComponent->GetHealthPercent();
}

float UFMBPlayerHUDWidget::GetStaminaPercent() const
{
    const auto StaminaComponent{FMBUtils::GetFMBPlayerComponent<UFMBStaminaComponent>(GetOwningPlayerPawn())};
    if (!StaminaComponent) return 0.0f;

    return StaminaComponent->GetStaminaPercent();
}

bool UFMBPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& WeaponUIData) const
{
    const auto WeaponComponent{FMBUtils::GetFMBPlayerComponent<UFMBBaseWeaponComponent>(GetOwningPlayerPawn())};
    if (!WeaponComponent) return false;

    // WeaponUIData.WeaponName = CheckWeaponName(WeaponUIData);

    return WeaponComponent->GetCurrentWeaponUIData(WeaponUIData);
}

/*bool UFMBPlayerHUDWidget::GetArmoryWeaponUIData(FWeaponUIData& WeaponUIData) const
{
    const auto WeaponComponent{FMBUtils::GetFMBPlayerComponent<UFMBBaseWeaponComponent>(GetOwningPlayerPawn())};
    if (!WeaponComponent) return false;

    // WeaponUIData.WeaponName = CheckWeaponName(WeaponUIData);

    return WeaponComponent->GetArmoryWeaponUIData(WeaponUIData);
}*/

ESlateVisibility UFMBPlayerHUDWidget::IsPlayerAlive() const
{
    const auto HealthComponent{FMBUtils::GetFMBPlayerComponent<UFMBHealthComponent>(GetOwningPlayerPawn())};
    return HealthComponent && !HealthComponent->IsDead() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
}

ESlateVisibility UFMBPlayerHUDWidget::IsPlayerSpectating() const
{
    return GetOwningPlayer() && GetOwningPlayer()->GetStateName() == NAME_Spectating ? ESlateVisibility::Visible
                                                                                     : ESlateVisibility::Collapsed;
}

bool UFMBPlayerHUDWidget::IsFPPCamera() const
{
    const auto Character{Cast<AFMBPlayerCharacter>(GetOwningPlayerPawn())};
    return Character && Character->GetIsFPP() /* ? ESlateVisibility::Visible : ESlateVisibility::Collapsed*/;
}

ESlateVisibility UFMBPlayerHUDWidget::CrossHairVisibility() const
{
    return (GetOwningPlayer() && GetOwningPlayer()->GetStateName() == NAME_Spectating) || IsFPPCamera() ? ESlateVisibility::Visible
                                                                                                        : ESlateVisibility::Collapsed;
}
