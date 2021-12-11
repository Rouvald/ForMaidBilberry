// For Maid Bilberry Game. All Rights Recerved


#include "AI/Services/FMBFindEnemyService.h"
#include "AIController.h"
#include "FMBUtils.h"
#include "FMBAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UFMBFindEnemyService::UFMBFindEnemyService()
{
    NodeName = "Find Enemy";
}

void UFMBFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if(Blackboard)
    {
        const auto Controller = OwnerComp.GetAIOwner();
        const auto PerceptionComponent = FMBUtils::GetFMBPlayerComponent<UFMBAIPerceptionComponent>(Controller);
        if (PerceptionComponent)
        {
            Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetClosestEnemy());

            const auto Distance = Controller->GetPawn()->GetDistanceTo(PerceptionComponent->GetClosestEnemy());
            Blackboard->SetValueAsFloat(DistanceToTargetKey.SelectedKeyName, Distance);
        }
    }
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}