// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "FMBSwordTrailAnimNotifyState.generated.h"

class UNiagaraComponent;
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBSwordTrailAnimNotifyState : public UAnimNotifyState
{
    GENERATED_BODY()

public:
    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

protected:
    UNiagaraComponent* GetSwordTrailFXComponent(USkeletalMeshComponent* MeshComp) const;
};
