// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FMBAttackTask.generated.h"

/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBAttackTask : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UFMBAttackTask();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector EnemyActorKey;
};
