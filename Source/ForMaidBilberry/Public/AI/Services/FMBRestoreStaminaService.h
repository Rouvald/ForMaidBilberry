// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "FMBRestoreStaminaService.generated.h"

/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBRestoreStaminaService : public UBTService
{
    GENERATED_BODY()

public:
    UFMBRestoreStaminaService();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector AIStaminaKey;

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
