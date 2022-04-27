// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FMBCharacterMovementComponent.generated.h"

class AFMBBaseCharacter;
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

    bool CanRolling() const;

    void OnRollingFinished(USkeletalMeshComponent* MeshComp);

protected:
    UPROPERTY()
    AFMBBaseCharacter* Character{nullptr};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "1.5", ClampMax = "5.0"))
    float RunModifier{2.0f};

    bool RollingAnimInProgress{false};
};
