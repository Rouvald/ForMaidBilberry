// For Maid Bilberry Game. All Rights Reserved

#include "AI/Tasks/FMBAttackTask.h"
#include "AIController.h"
#include "FMBUtils.h"
#include "FMBAIPerceptionComponent.h"
#include "FMBBaseWeaponComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

// DEFINE_LOG_CATEGORY_STATIC(LogFMBAttackTask, All, All)

UFMBAttackTask::UFMBAttackTask()
{
    NodeName = "Attack Enemy";
}

EBTNodeResult::Type UFMBAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Controller || !Blackboard) return EBTNodeResult::Failed;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return EBTNodeResult::Failed;

    const auto PerceiveComponent = FMBUtils::GetFMBPlayerComponent<UFMBAIPerceptionComponent>(Controller);
    if (!PerceiveComponent) return EBTNodeResult::Failed;

    const auto WeaponComponent = FMBUtils::GetFMBPlayerComponent<UFMBBaseWeaponComponent>(Pawn);
    const auto PerceiveActor = PerceiveComponent->GetEnemyPlayer();
    if (!WeaponComponent || !PerceiveActor) return EBTNodeResult::Failed;

    FMath::RandBool() ? WeaponComponent->MeleeAttack(EStaminaSpend::ESS_FastAttack)
                      : WeaponComponent->MeleeAttack(EStaminaSpend::ESS_StrongAttack);

    return EBTNodeResult::Succeeded;
}
