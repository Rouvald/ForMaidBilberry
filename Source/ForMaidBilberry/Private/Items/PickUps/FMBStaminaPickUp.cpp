// For Maid Bilberry Game. All Rights Reserved

#include "Items/PickUps/FMBStaminaPickUp.h"
#include "Components/FMBStaminaComponent.h"
#include "FMBPlayerCharacter.h"
#include "Components/WidgetComponent.h"
#include "UI/FMBItemInfoWidget.h"
#include "Components/FMBPlayerWeaponComponent.h"

void AFMBStaminaPickUp::SetItemInfo() const
{
    Super::SetItemInfo();

    const auto ItemInfoWidget{Cast<UFMBItemInfoWidget>(ItemInfoWidgetComponent->GetWidget())};
    if (ItemInfoWidget)
    {
        ItemInfoWidget->SetItemProperty(StaminaAmount);
    }
}

void AFMBStaminaPickUp::UpdateItemInfoProperty(const AFMBPlayerCharacter* PlayerCharacter) const
{
    if (!PlayerCharacter) return;

    const auto WeaponComponent{PlayerCharacter->FindComponentByClass<UFMBPlayerWeaponComponent>()};
    if (!WeaponComponent) return;

    const auto ItemInfoWidget{Cast<UFMBItemInfoWidget>(ItemInfoWidgetComponent->GetWidget())};
    if (!ItemInfoWidget) return;

    if (WeaponComponent->GetCurrentPickUp())
    {
        if (WeaponComponent->GetCurrentPickUp()->GetPickUpType() != EPickUpType::EPT_StaminaFlask) return;

        const auto StaminaPickUp{Cast<AFMBStaminaPickUp>(WeaponComponent->GetCurrentPickUp())};
        if (!StaminaPickUp) return;

        ItemInfoWidget->SetItemProperty(StaminaAmount - StaminaPickUp->GetStaminaAmount());
    }
    else
    {
        ItemInfoWidget->SetItemProperty(StaminaAmount);
    }
}

bool AFMBStaminaPickUp::CanUsePickUp(APawn* Pawn)
{
    const auto StaminaComponent{Pawn->FindComponentByClass<UFMBStaminaComponent>()};
    if (!StaminaComponent) return false;

    return StaminaComponent->TryAddStamina(StaminaAmount);
}