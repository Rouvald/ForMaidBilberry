// For Maid Bilberry Game. All Rights Reserved

#include "AI/Services/FMBAttackService.h"
#include "AIController.h"
#include "FMBUtils.h"
#include "Components/FMBBaseWeaponComponent.h"
#include "FMBAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UFMBAttackService::UFMBAttackService()
{
    NodeName = "Attack";
}

void UFMBAttackService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto Blackboard = OwnerComp.GetBlackboardComponent();

    if (Controller && Blackboard)
    {
        const auto PerceiveComponent = FMBUtils::GetFMBPlayerComponent<UFMBAIPerceptionComponent>(Controller);
        if (PerceiveComponent)
        {
            const auto WeaponComponent = FMBUtils::GetFMBPlayerComponent<UFMBBaseWeaponComponent>(Controller->GetPawn());
            const auto PerceiveActor = PerceiveComponent->GetEnemyPlayer();
            if (WeaponComponent && PerceiveActor)
            {
                const auto CurrentDistance = (PerceiveActor->GetActorLocation() - Controller->GetPawn()->GetActorLocation()).Size();
                if (CurrentDistance <= AttackDistance)
                {
                    FMath::RandBool() ? WeaponComponent->MeleeAttack(EStaminaSpend::ESS_FastAttack)
                                      : WeaponComponent->MeleeAttack(EStaminaSpend::ESS_StrongAttack);
                }
            }
        }
    }
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
