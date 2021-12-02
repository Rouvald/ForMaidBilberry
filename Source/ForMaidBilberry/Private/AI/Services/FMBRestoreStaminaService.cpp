// For Maid Bilberry Game. All Rights Recerved


#include "AI/Services/FMBRestoreStaminaService.h"
#include "AIController.h"
#include "Components/FMBHealthComponent.h"
#include "FMBUtils.h"
#include "BehaviorTree/BlackboardComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBRestoreStaminaService, All, All)

UFMBRestoreStaminaService::UFMBRestoreStaminaService()
{
    NodeName = TEXT("Restore Stamina");
}

void UFMBRestoreStaminaService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto Blackboard = OwnerComp.GetBlackboardComponent();

    if (Controller && Blackboard)
    {
        const auto HealthComponent = FMBUtils::GetFMBPlayerComponent<UFMBHealthComponent>(Controller->GetPawn());
        if (HealthComponent)
        {
            Blackboard->SetValueAsFloat(AIStaminaKey.SelectedKeyName, HealthComponent->GetStamina());
            //UE_LOG(LogFMBRestoreStaminaService, Display, TEXT("Stamina: %f"), Blackboard->GetValueAsFloat(AIStaminaKey.SelectedKeyName));
        }
    }
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
