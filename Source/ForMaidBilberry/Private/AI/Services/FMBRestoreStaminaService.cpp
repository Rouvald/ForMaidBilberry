// For Maid Bilberry Game. All Rights Reserved

#include "AI/Services/FMBRestoreStaminaService.h"
#include "AIController.h"
#include "Components/FMBStaminaComponent.h"
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
        const auto StaminaComponent = FMBUtils::GetFMBPlayerComponent<UFMBStaminaComponent>(Controller->GetPawn());
        if (StaminaComponent)
        {
            Blackboard->SetValueAsFloat(AIStaminaKey.SelectedKeyName, StaminaComponent->GetStamina());
            // UE_LOG(LogFMBRestoreStaminaService, Display, TEXT("Stamina: %f"), Blackboard->GetValueAsFloat(AIStaminaKey.SelectedKeyName));
        }
    }
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
