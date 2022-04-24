// For Maid Bilberry Game. All Rights Recerved

#include "PickUps/FMBHealthPickUp.h"

#include "FMBHealthComponent.h"
#include "FMBPlayerCharacter.h"
#include "Components/WidgetComponent.h"
#include "UI/FMBItemInfoWidget.h"

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
        ItemInfoWidget->SetItemHealthAmount(HealthAmount);
    }
}

bool AFMBHealthPickUp::CanUsePickUp(APawn* Pawn)
{
    const auto HealthComponent{Pawn->FindComponentByClass<UFMBHealthComponent>()};
    if (!HealthComponent) return false;

    return HealthComponent->TryToAddHealth(HealthAmount);
}
