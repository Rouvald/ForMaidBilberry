// For Maid Bilberry Game. All Rights Reserved

#include "UI/FMBHealthPickupWidget.h"

void UFMBHealthPickupWidget::StartPlayingAnim()
{
    if (!IsAnimationPlaying(BlinkingAnim))
    {
        PlayAnimation(BlinkingAnim);
    }
}