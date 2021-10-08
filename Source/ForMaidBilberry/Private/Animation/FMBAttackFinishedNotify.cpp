// For Maid Bilberry Game. All Rights Recerved


#include "Animation/FMBAttackFinishedNotify.h"

void UFMBAttackFinishedNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    OnNotified.Broadcast(MeshComp);
    Super::Notify(MeshComp, Animation);
}