// For Maid Bilberry Game. All Rights Reserved

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
public:
    FORCEINLINE float GetHealthAmount() const { return HealthAmount; };

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickUp", meta = (ClampMin = "10.0", ClampMax = "90.0"))
    float HealthAmount{50.0f};

    virtual void SetItemInfo() const override;
    virtual void UpdateItemInfoProperty(const AFMBPlayerCharacter* PlayerCharacter) const override;

    virtual bool CanUsePickUp(APawn* Pawn) override;
};
