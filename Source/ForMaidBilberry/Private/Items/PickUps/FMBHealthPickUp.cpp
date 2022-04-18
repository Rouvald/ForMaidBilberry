// For Maid Bilberry Game. All Rights Recerved

#include "PickUps/FMBHealthPickUp.h"

#include "FMBHealthComponent.h"
#include "FMBPlayerCharacter.h"
#include "FMBUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBHealthPickUp, All, All)

/*bool AFMBHealthPickUp::GivePickUpTo(APawn* Pawn)
{
    const auto HealthComponent = FMBUtils::GetFMBPlayerComponent<UFMBHealthComponent>(Pawn);
    if (!HealthComponent) return false;

    return HealthComponent->TryToAddHealth(HealthAmount);
}*/

bool AFMBHealthPickUp::CanUsePickUp(APawn* Pawn)
{
    const auto HealthComponent{Pawn->FindComponentByClass<UFMBHealthComponent>()};
    if (!HealthComponent) return false;

    return HealthComponent->TryToAddHealth(HealthAmount);
}
