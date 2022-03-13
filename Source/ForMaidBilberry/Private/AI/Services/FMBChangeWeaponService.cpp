// For Maid Bilberry Game. All Rights Recerved

#include "AI/Services/FMBChangeWeaponService.h"

#include "AIController.h"
#include "Components/FMBBaseWeaponComponent.h"
#include "FMBUtils.h"

UFMBChangeWeaponService::UFMBChangeWeaponService()
{
    NodeName = "Change Weapon";
}

void UFMBChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (Controller)
    {
        const auto WeaponComponent = FMBUtils::GetFMBPlayerComponent<UFMBBaseWeaponComponent>(Controller->GetPawn());
        if (WeaponComponent && Probability > 0 && FMath::FRand() >= Probability)
        {
            // WeaponComponent->NextWeapon();
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
