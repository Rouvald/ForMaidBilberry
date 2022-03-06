// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "FMBAnimNotifyState.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifyStateBeginSignature, USkeletalMeshComponent*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifyStateEndSignature, USkeletalMeshComponent*);
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBAnimNotifyState : public UAnimNotifyState
{
    GENERATED_BODY()
public:
    FOnNotifyStateBeginSignature OnNotifyStateBegin;
    FOnNotifyStateEndSignature OnNotifyStateEnd;

    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
