// For Maid Bilberry Game. All Rights Reserved

#include "AI/Tasks/FMBRotationTask.h"
#include "AIController.h"
#include "FMBUtils.h"
#include "FMBAIPerceptionComponent.h"
#include "FMBBaseWeaponComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UFMBRotationTask::UFMBRotationTask()
{
    NodeName = "Rotation";
}

EBTNodeResult::Type UFMBRotationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto Blackboard = OwnerComp.GetBlackboardComponent();

    if (!Controller) return EBTNodeResult::Failed;
    const auto Pawn = Controller->GetPawn();

    if (!Pawn || !Blackboard) return EBTNodeResult::Failed;

    const auto StartAILocation = Blackboard->GetValueAsVector(LocationActorKey.SelectedKeyName);
    Pawn->SetActorRotation(FRotator(0.0f, StartAILocation.Rotation().Yaw, 0.0f));

    return EBTNodeResult::Succeeded;
}
