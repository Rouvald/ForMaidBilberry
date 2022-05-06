// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "PickUps/FMBBasePickUp.h"
#include "FMBFillPropertyPickUp.generated.h"

/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API AFMBFillPropertyPickUp : public AFMBBasePickUp
{
    GENERATED_BODY()
public:
    FORCEINLINE float GetFillPropertyAmount() const { return FillPropertyAmount; };

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickUp", meta = (ClampMin = "10.0", ClampMax = "90.0"))
    float FillPropertyAmount{50.0f};

    virtual void SetItemInfo() const override;
    virtual void UpdateItemInfoProperty(const AFMBPlayerCharacter* CurrentPlayerCharacter) const override;

    virtual bool CanUsePickUp(APawn* Pawn) override;
};
