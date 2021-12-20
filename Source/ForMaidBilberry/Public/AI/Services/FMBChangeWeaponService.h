// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "FMBChangeWeaponService.generated.h"

/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBChangeWeaponService : public UBTService
{
    GENERATED_BODY()
public:
    UFMBChangeWeaponService();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float Probability = 0.5f;

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
