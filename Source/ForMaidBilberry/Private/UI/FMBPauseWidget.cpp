// For Maid Bilberry Game. All Rights Recerved

#include "UI/FMBPauseWidget.h"
#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"

void UFMBPauseWidget::NativeOnInitialized()
{
    if (ClearPauseButton)
    {
        ClearPauseButton->OnClicked.AddDynamic(this, &UFMBPauseWidget::OnPauseClear);
    }
}

void UFMBPauseWidget::OnPauseClear()
{
    if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

    GetWorld()->GetAuthGameMode()->ClearPause();
}