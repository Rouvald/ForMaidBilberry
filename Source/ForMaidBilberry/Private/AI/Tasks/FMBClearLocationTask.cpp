// For Maid Bilberry Game. All Rights Reserved

#include "AI/Tasks/FMBClearLocationTask.h"
#include "BehaviorTree/BlackboardComponent.h"

UFMBClearLocationTask::UFMBClearLocationTask()
{
    NodeName = "Clear Location";
}

EBTNodeResult::Type UFMBClearLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Blackboard) return EBTNodeResult::Failed;

    Blackboard->ClearValue(LocationToClearKey.SelectedKeyName);

    return EBTNodeResult::Succeeded;
}
