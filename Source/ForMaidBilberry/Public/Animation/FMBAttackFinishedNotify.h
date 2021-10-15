// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "FMBAttackFinishedNotify.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAttackFinishedNotifySignature, USkeletalMeshComponent*);

UCLASS()
class FORMAIDBILBERRY_API UFMBAttackFinishedNotify : public UAnimNotify
{
    GENERATED_BODY()

public:
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

    FOnAttackFinishedNotifySignature OnAttackFinishedNotify;
};
