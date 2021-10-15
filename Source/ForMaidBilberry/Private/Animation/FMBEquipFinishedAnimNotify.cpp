// For Maid Bilberry Game. All Rights Recerved


#include "Animation/FMBEquipFinishedAnimNotify.h"

void UFMBEquipFinishedAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    OnEquipFinishedNotify.Broadcast(MeshComp);
    Super::Notify(MeshComp, Animation);
}