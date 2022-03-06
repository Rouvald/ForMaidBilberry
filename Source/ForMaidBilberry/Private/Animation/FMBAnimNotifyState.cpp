// For Maid Bilberry Game. All Rights Reserved

#include "Animation/FMBAnimNotifyState.h"

void UFMBAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    OnNotifyStateBegin.Broadcast(MeshComp);
    Super::NotifyBegin(MeshComp, Animation, TotalDuration);
}
void UFMBAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    OnNotifyStateEnd.Broadcast(MeshComp);
    Super::NotifyEnd(MeshComp, Animation);
}