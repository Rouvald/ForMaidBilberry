// For Maid Bilberry Game. All Rights Recerved

#include "UI/FMBHealthPickupWidget.h"

void UFMBHealthPickupWidget::StartPlayingAnim()
{
    if (!IsAnimationPlaying(BlinkingAnim))
    {
        PlayAnimation(BlinkingAnim);
    }
}