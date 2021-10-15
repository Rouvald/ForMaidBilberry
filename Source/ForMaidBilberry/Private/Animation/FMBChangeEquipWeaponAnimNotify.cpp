// For Maid Bilberry Game. All Rights Recerved


#include "Animation/FMBChangeEquipWeaponAnimNotify.h"

void UFMBChangeEquipWeaponAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    OnChangeEquipWeapon.Broadcast(MeshComp);
    Super::Notify(MeshComp, Animation);
}