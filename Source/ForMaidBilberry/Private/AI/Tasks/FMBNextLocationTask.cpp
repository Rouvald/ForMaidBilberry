// For Maid Bilberry Game. All Rights Recerved


#include "AI/Tasks/FMBNextLocationTask.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"

UFMBNextLocationTask::UFMBNextLocationTask()
{
    NodeName = "Next Location";
}

EBTNodeResult::Type UFMBNextLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Controller || !Blackboard) return EBTNodeResult::Failed;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return EBTNodeResult::Failed;

    const auto NavSys = UNavigationSystemV1::GetCurrent(Pawn);
    if (!NavSys) return EBTNodeResult::Failed;

    FNavLocation NavLocation;
    auto Location = Pawn->GetActorLocation();
    if (!SelfCenter)
    {
        const auto CenterActor = Cast<AActor>(Blackboard->GetValueAsObject(CenterActorKey.SelectedKeyName));
        if (!CenterActor) return EBTNodeResult::Failed;
        Location = CenterActor->GetActorLocation();
    }
    const auto bFound = NavSys->GetRandomReachablePointInRadius(Location, Radius, NavLocation);
    if (!bFound) return EBTNodeResult::Failed;

    Blackboard->SetValueAsVector(AimLocation.SelectedKeyName, NavLocation.Location);
    return EBTNodeResult::Succeeded;
}
