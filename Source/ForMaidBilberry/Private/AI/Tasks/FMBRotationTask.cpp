// For Maid Bilberry Game. All Rights Recerved


#include "AI/Tasks/FMBRotationTask.h"
#include "AIController.h"
#include "FMBUtils.h"
#include "FMBAIPerceptionComponent.h"
#include "FMBWeaponComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UFMBRotationTask::UFMBRotationTask()
{
    NodeName = "Rotation";
}

EBTNodeResult::Type UFMBRotationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Controller || !Blackboard) return EBTNodeResult::Failed;

    const auto StartAILocation = Blackboard->GetValueAsVector(LocationActorKey.SelectedKeyName);
    Controller->SetControlRotation(StartAILocation.Rotation());
    
    return EBTNodeResult::Succeeded;
}
