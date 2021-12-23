// For Maid Bilberry Game. All Rights Recerved

#include "UI/FMBBaseWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void UFMBBaseWidget::Show()
{
    PlayAnimation(ShowAnim);
    UGameplayStatics::PlaySound2D(GetWorld(), OpenWidgetSound);
}