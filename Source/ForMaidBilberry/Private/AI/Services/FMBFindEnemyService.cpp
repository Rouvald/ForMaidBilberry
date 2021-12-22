// For Maid Bilberry Game. All Rights Recerved

#include "AI/Services/FMBFindEnemyService.h"
#include "AIController.h"
#include "FMBAIPerceptionComponent.h"
#include "FMBHealthComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "FMBUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBFindenemyService, All, All)

UFMBFindEnemyService::UFMBFindEnemyService()
{
    NodeName = "Find Enemy";
}

void UFMBFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (GetWorld() && Blackboard)
    {
        const auto Controller = OwnerComp.GetAIOwner();
        const auto PerceptionComponent = FMBUtils::GetFMBPlayerComponent<UFMBAIPerceptionComponent>(Controller);
        if (PerceptionComponent)
        {
            Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetEnemyPlayer());

            if (PerceptionComponent->GetEnemyPlayer())
            {
                Blackboard->SetValueAsVector(
                    LastPlayerLocationKey.SelectedKeyName, PerceptionComponent->GetEnemyPlayer()->GetActorLocation());
            }
            /*const auto Distance = Controller->GetPawn()->GetDistanceTo(PerceptionComponent->GetEnemyPlayer());
            Blackboard->SetValueAsFloat(DistanceToTargetKey.SelectedKeyName, Distance);*/
        }
    }
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
