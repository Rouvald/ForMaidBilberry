// For Maid Bilberry Game. All Rights Recerved


#include "UI/FMBPauseWidget.h"
#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"

bool UFMBPauseWidget::Initialize()
{
    const auto InitStatus = Super::Initialize();

    if(ClearPauseButton)
    {
        ClearPauseButton->OnClicked.AddDynamic(this, &UFMBPauseWidget::OnPauseClear);
    }
    return InitStatus;
}

void UFMBPauseWidget::OnPauseClear()
{
    if(!GetWorld() || !GetWorld()->GetAuthGameMode()) return;
    
    GetWorld()->GetAuthGameMode()->ClearPause();
}