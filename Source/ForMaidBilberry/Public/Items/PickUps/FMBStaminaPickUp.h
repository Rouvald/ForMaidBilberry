// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Items/PickUps/FMBBasePickUp.h"
#include "FMBStaminaPickUp.generated.h"

/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API AFMBStaminaPickUp : public AFMBBasePickUp
{
    GENERATED_BODY()
public:
    FORCEINLINE float GetStaminaAmount() const { return StaminaAmount; };

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickUp", meta = (ClampMin = "10.0", ClampMax = "90.0"))
    float StaminaAmount{50.0f};

    virtual void SetItemInfo() const override;
    virtual void UpdateItemInfoProperty(const AFMBPlayerCharacter* PlayerCharacter) const override;

    virtual bool CanUsePickUp(APawn* Pawn) override;
};
