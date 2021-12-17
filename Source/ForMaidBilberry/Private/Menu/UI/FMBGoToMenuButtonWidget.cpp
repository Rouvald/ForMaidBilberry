// For Maid Bilberry Game. All Rights Recerved


#include "Menu/UI/FMBGoToMenuButtonWidget.h"
#include "Components/Button.h"
#include "FMBMenuGameModeBase.h"

void UFMBGoToMenuButtonWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (BackToMenuButton)
    {
        BackToMenuButton->OnClicked.AddDynamic(this, &UFMBGoToMenuButtonWidget::OnBackToMenu);
    }
}

void UFMBGoToMenuButtonWidget::OnBackToMenu()
{
    if (GetWorld())
    {
        const auto GameMode = Cast<AFMBMenuGameModeBase>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->SetMenuState(EFMBMenuState::MainMenu);
        }
    }
}