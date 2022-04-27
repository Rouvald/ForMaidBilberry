// For Maid Bilberry Game. All Rights Reserved

#include "PickUps/FMBHealthPickUp.h"

#include "FMBHealthComponent.h"
#include "FMBPlayerCharacter.h"
#include "Components/WidgetComponent.h"
#include "UI/FMBItemInfoWidget.h"
#include "Components/FMBPlayerWeaponComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBHealthPickUp, All, All)

/*bool AFMBHealthPickUp::GivePickUpTo(APawn* Pawn)
{
    const auto HealthComponent = FMBUtils::GetFMBPlayerComponent<UFMBHealthComponent>(Pawn);
    if (!HealthComponent) return false;

    return HealthComponent->TryToAddHealth(HealthAmount);
}*/

void AFMBHealthPickUp::SetItemInfo() const
{
    Super::SetItemInfo();

    const auto ItemInfoWidget{Cast<UFMBItemInfoWidget>(ItemInfoWidgetComponent->GetWidget())};
    if (ItemInfoWidget)
    {
        ItemInfoWidget->SetItemProperty(HealthAmount);
    }
}

void AFMBHealthPickUp::UpdateItemInfoProperty(const AFMBPlayerCharacter* PlayerCharacter) const
{
    if (!PlayerCharacter) return;

    const auto WeaponComponent{PlayerCharacter->FindComponentByClass<UFMBPlayerWeaponComponent>()};
    if (!WeaponComponent) return;

    const auto ItemInfoWidget{Cast<UFMBItemInfoWidget>(ItemInfoWidgetComponent->GetWidget())};
    if (!ItemInfoWidget) return;

    if (WeaponComponent->GetCurrentPickUp())
    {
        if (WeaponComponent->GetCurrentPickUp()->GetPickUpType() != EPickUpType::EPT_HealthFlask) return;

        const auto HealthPickUp{Cast<AFMBHealthPickUp>(WeaponComponent->GetCurrentPickUp())};
        if (!HealthPickUp) return;

        ItemInfoWidget->SetItemProperty(HealthAmount - HealthPickUp->GetHealthAmount());
    }
    else
    {
        ItemInfoWidget->SetItemProperty(HealthAmount);
    }
}

bool AFMBHealthPickUp::CanUsePickUp(APawn* Pawn)
{
    const auto HealthComponent{Pawn->FindComponentByClass<UFMBHealthComponent>()};
    if (!HealthComponent) return false;

    return HealthComponent->TryToAddHealth(HealthAmount);
}
