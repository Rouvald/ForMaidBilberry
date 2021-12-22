// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "FMBFindEnemyService.generated.h"

/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBFindEnemyService : public UBTService
{
    GENERATED_BODY()

public:
    UFMBFindEnemyService();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector EnemyActorKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector LastPlayerLocationKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector DistanceToPlayerKey;

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
