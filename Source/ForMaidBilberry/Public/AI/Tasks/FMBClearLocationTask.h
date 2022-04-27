// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FMBClearLocationTask.generated.h"

/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBClearLocationTask : public UBTTaskNode
{
    GENERATED_BODY()
public:
    UFMBClearLocationTask();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector LocationToClearKey;
};
