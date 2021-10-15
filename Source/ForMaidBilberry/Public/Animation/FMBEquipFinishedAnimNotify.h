// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "FMBEquipFinishedAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquipFinishedSignature, USkeletalMeshComponent*);

UCLASS()
class FORMAIDBILBERRY_API UFMBEquipFinishedAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
    public:
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

    FOnEquipFinishedSignature OnEquipFinishedNotify;
};
