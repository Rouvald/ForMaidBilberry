// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "FMBChangeEquipWeaponAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangeEquipWeaponSignature, USkeletalMeshComponent*);

UCLASS()
class FORMAIDBILBERRY_API UFMBChangeEquipWeaponAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
    public:
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

    FOnChangeEquipWeaponSignature OnChangeEquipWeapon;
};
