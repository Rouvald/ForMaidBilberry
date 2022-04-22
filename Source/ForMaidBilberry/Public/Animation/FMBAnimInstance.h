// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FMBCoreTypes.h"
#include "FMBAnimInstance.generated.h"

class AFMBBaseCharacter;
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBAnimInstance : public UAnimInstance
{
    GENERATED_BODY()
public:
    virtual void NativeInitializeAnimation() override;

    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    AFMBBaseCharacter* BaseCharacter{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    float Velocity{0.0f};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    float MovementDirection{0.0f};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    bool bIsFalling{false};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    bool bIsRunning{false};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    bool bIsCurrentWeapon{false};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    bool bIsBlocking{false};

    /*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    FWeaponAnimationsData CurrentWeaponAnimData;*/

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    EWeaponType CurrentWeaponType;

    void UpdateAnimationProperties(float DeltaSeconds);

    float GetMovementDirection() const;
};
