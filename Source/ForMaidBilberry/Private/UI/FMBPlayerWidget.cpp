// For Maid Bilberry Game. All Rights Reserved

#include "UI/FMBPlayerWidget.h"

#include "FMBPlayerCharacter.h"
#include "Components/FMBHealthComponent.h"
#include "Components/FMBStaminaComponent.h"
#include "FMBUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBPlayerHUDWidget, All, All)

void UFMBPlayerWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (GetOwningPlayer())
    {
        GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &UFMBPlayerWidget::OnNewPawn);
        OnNewPawn(GetOwningPlayerPawn());
    }
}

void UFMBPlayerWidget::OnNewPawn(APawn* NewPawn)
{
    const auto HealthComponent{FMBUtils::GetFMBPlayerComponent<UFMBHealthComponent>(NewPawn)};
    if (HealthComponent)
    {
        HealthComponent->OnHealthChange.AddUObject(this, &UFMBPlayerWidget::OnHealthChange);
    }
}

void UFMBPlayerWidget::OnHealthChange(float Health, float HealthDelta)
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

float UFMBPlayerWidget::GetHealthPercent() const
{
    const auto HealthComponent{FMBUtils::GetFMBPlayerComponent<UFMBHealthComponent>(GetOwningPlayerPawn())};
    if (!HealthComponent) return 0.0f;

    return HealthComponent->GetHealthPercent();
}

float UFMBPlayerWidget::GetStaminaPercent() const
{
    const auto StaminaComponent{FMBUtils::GetFMBPlayerComponent<UFMBStaminaComponent>(GetOwningPlayerPawn())};
    if (!StaminaComponent) return 0.0f;

    return StaminaComponent->GetStaminaPercent();
}

/*UTexture2D* UFMBPlayerWidget::GetCurrentWeaponUIImage() const
{
    const auto WeaponComponent{FMBUtils::GetFMBPlayerComponent<UFMBBaseWeaponComponent>(GetOwningPlayerPawn())};
    if (!WeaponComponent) return nullptr;

    // WeaponUIData.WeaponName = CheckWeaponName(WeaponUIData);

    return WeaponComponent->GetCurrentWeaponUIImage();
}*/

/*bool UFMBPlayerWidget::GetArmoryWeaponUIData(FItemData& WeaponUIData) const
{
    const auto WeaponComponent{FMBUtils::GetFMBPlayerComponent<UFMBBaseWeaponComponent>(GetOwningPlayerPawn())};
    if (!WeaponComponent) return false;

    // WeaponUIData.WeaponName = CheckWeaponName(WeaponUIData);

    return WeaponComponent->GetArmoryWeaponUIData(WeaponUIData);
}*/

ESlateVisibility UFMBPlayerWidget::IsPlayerAlive() const
{
    const auto HealthComponent{FMBUtils::GetFMBPlayerComponent<UFMBHealthComponent>(GetOwningPlayerPawn())};
    return HealthComponent && !HealthComponent->IsDead() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
}

ESlateVisibility UFMBPlayerWidget::IsPlayerSpectating() const
{
    return GetOwningPlayer() && GetOwningPlayer()->GetStateName() == NAME_Spectating ? ESlateVisibility::Visible
                                                                                     : ESlateVisibility::Collapsed;
}

/*bool UFMBPlayerWidget::IsFPPCamera() const
{
    const auto Character{Cast<AFMBPlayerCharacter>(GetOwningPlayerPawn())};
    return Character && Character->GetIsFPP() /* ? ESlateVisibility::Visible : ESlateVisibility::Collapsed#1#;
}*/

/*ESlateVisibility UFMBPlayerWidget::CrossHairVisibility() const
{
    return (GetOwningPlayer() && GetOwningPlayer()->GetStateName() == NAME_Spectating) || IsFPPCamera() ? ESlateVisibility::Visible
                                                                                                        : ESlateVisibility::Collapsed;
}*/
