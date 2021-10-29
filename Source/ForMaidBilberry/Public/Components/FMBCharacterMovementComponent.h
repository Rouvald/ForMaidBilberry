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
    virtual float GetMaxSpeed() const override;

    virtual void BeginPlay() override;

    void Rolling();

    //bool GetRollingAnimInProgress() const { return RollingAnimInProgress; }

    bool CanRolling() const;

protected:
    UPROPERTY(EditDefaultsOnly, Category="Animation")
    UAnimMontage* RollingAnimMontage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement", meta = (ClampMin="1.5", ClampMax="5.0"))
    float RunModifier = 2.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement")
    float RollingVelocity = 1000.0f;

    //UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement", meta = (ClampMin="2.0", ClampMax="7.0"))
    //float RollingModifier = 4.0f;

    bool RollingAnimInProgress = false;

    void InitAnimation();

    void OnRollingFinished(USkeletalMeshComponent* MeshComp);
};
