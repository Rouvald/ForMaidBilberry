// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FMBCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

    public:

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement", meta = (ClampMin="1.5", ClampMax="5.0"))
    float RunModifier = 2.0f;
        
    virtual float GetMaxSpeed() const override;
};
