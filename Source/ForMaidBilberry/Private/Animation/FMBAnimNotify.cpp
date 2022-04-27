// For Maid Bilberry Game. All Rights Reserved

#include "Animation/FMBAnimNotify.h"

void UFMBAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    OnNotify.Broadcast(MeshComp);
    Super::Notify(MeshComp, Animation);
}