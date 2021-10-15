// For Maid Bilberry Game. All Rights Recerved


#include "Animation/FMBAttackFinishedNotify.h"

void UFMBAttackFinishedNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    OnAttackFinishedNotify.Broadcast(MeshComp);
    Super::Notify(MeshComp, Animation);
}