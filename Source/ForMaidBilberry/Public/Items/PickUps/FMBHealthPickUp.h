// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "PickUps/FMBBasePickUp.h"
#include "FMBHealthPickUp.generated.h"

/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API AFMBHealthPickUp : public AFMBBasePickUp
{
    GENERATED_BODY()
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickUp", meta = (ClampMin = "10.0", ClampMax = "100.0"))
    float HealthAmount{50.0f};

    virtual void SetItemInfo() const override;

    virtual bool CanUsePickUp(APawn* Pawn) override;
};
