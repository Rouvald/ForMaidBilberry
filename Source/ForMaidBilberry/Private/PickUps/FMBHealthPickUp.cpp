// For Maid Bilberry Game. All Rights Recerved


#include "PickUps/FMBHealthPickUp.h"

#include "FMBHealthComponent.h"
#include "FMBUtils.h"

DEFINE_LOG_CATEGORY_STATIC(HealthPickUpLog, All, All)

bool AFMBHealthPickUp::GivePickUpTo(APawn* Pawn)
{
    const auto HealthComponent=  FMBUtils::GetFMBPlayerComponent<UFMBHealthComponent>(Pawn);
    if(!HealthComponent) return false;

    return HealthComponent->TryToAddHealth(HealthAmount);
}