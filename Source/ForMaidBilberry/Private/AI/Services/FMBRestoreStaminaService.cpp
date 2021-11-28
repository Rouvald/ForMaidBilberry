// For Maid Bilberry Game. All Rights Recerved


#include "AI/Services/FMBRestoreStaminaService.h"

#include "AIController.h"
#include "Components/FMBHealthComponent.h"
#include "FMBUtils.h"
#include "BehaviorTree/BlackboardComponent.h"

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
            UE_LOG(LogTemp, Display, TEXT("%f"), HealthComponent->GetStamina());
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
