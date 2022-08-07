// For Maid Bilberry Game. All Rights Reserved

#include "PickUps/FMBFillPropertyPickUp.h"
#include "Components/FMBHealthComponent.h"
#include "Components/FMBStaminaComponent.h"
#include "FMBPlayerCharacter.h"
#include "Components/WidgetComponent.h"
#include "UI/FMBItemInfoWidget.h"
#include "Components/FMBPlayerWeaponComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBHealthPickUp, All, All)

void AFMBFillPropertyPickUp::SetItemInfo() const
{
    Super::SetItemInfo();

    const auto ItemInfoWidget{Cast<UFMBItemInfoWidget>(ItemInfoWidgetComponent->GetWidget())};
    if (ItemInfoWidget)
    {
        ItemInfoWidget->SetItemProperty(FillPropertyAmount);
    }
}

void AFMBFillPropertyPickUp::UpdateItemInfoProperty(const AFMBPlayerCharacter* CurrentPlayerCharacter) const
{
    if (!CurrentPlayerCharacter) return;

    const auto WeaponComponent{CurrentPlayerCharacter->FindComponentByClass<UFMBPlayerWeaponComponent>()};
    if (!WeaponComponent) return;

    const auto ItemInfoWidget{Cast<UFMBItemInfoWidget>(ItemInfoWidgetComponent->GetWidget())};
    if (!ItemInfoWidget) return;

    if (WeaponComponent->GetCurrentPickUp() && WeaponComponent->GetCurrentPickUp()->GetPickUpType() == PickUpType)
    {
        const auto FillPropertyPickUp{Cast<AFMBFillPropertyPickUp>(WeaponComponent->GetCurrentPickUp())};
        if (!FillPropertyPickUp) return;

        ItemInfoWidget->SetItemProperty(FillPropertyAmount - FillPropertyPickUp->GetFillPropertyAmount());
    }
    else
    {
        ItemInfoWidget->SetItemProperty(FillPropertyAmount);
    }
}

bool AFMBFillPropertyPickUp::CanUsePickUp(APawn* Pawn)
{
    switch (PickUpType)
    {
        case EPickUpType::EPT_HealthFlask:
        {
            const auto HealthComponent{Pawn->FindComponentByClass<UFMBHealthComponent>()};
            if (!HealthComponent) return false;

            return HealthComponent->TryToAddHealth(FillPropertyAmount);
        }
        case EPickUpType::EPT_StaminaFlask:
        {
            const auto StaminaComponent{Pawn->FindComponentByClass<UFMBStaminaComponent>()};
            if (!StaminaComponent) return false;

            return StaminaComponent->TryAddStamina(FillPropertyAmount);
        }
        default: break;
    }
    return false;
}
