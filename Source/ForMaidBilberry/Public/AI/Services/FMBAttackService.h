// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "FMBAttackService.generated.h"

class UFMBWeaponComponent;
/**
 * 
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBAttackService : public UBTService
{
	GENERATED_BODY()
public:
    UFMBAttackService();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
    FBlackboardKeySelector EnemyActorKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
    float AttackDistance = 100.0f;

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
