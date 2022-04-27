// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FMBRotationTask.generated.h"

/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBRotationTask : public UBTTaskNode
{
    GENERATED_BODY()
public:
    UFMBRotationTask();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector LocationActorKey;
};
