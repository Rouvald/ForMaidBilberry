// For Maid Bilberry Game. All Rights Recerved

#include "Menu/UI/FMBMenuWidget.h"
#include "Menu/FMBMenuGameModeBase.h"
#include "FMBLevelItemWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBMenuWidget, All, All)

void UFMBMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (SelectLevelButton)
    {
        SelectLevelButton->OnClicked.AddDynamic(this, &UFMBMenuWidget::OnSelectLevelGame);
    }
    if (QuitGameButton)
    {
        QuitGameButton->OnClicked.AddDynamic(this, &UFMBMenuWidget::OnQuitGame);
    }
    /*if(SelectCharacterButton)
    {
        SelectCharacterButton->OnClicked.AddDynamic(this, &UFMBMenuWidget::OnSelectCharacterClicked);
    }*/
}

void UFMBMenuWidget::OnSelectLevelGame()
{
    if (GetWorld())
    {
        const auto GameMode = Cast<AFMBMenuGameModeBase>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->SetMenuState(EFMBMenuState::SelectLevel);
        }
    }
}

void UFMBMenuWidget::OnQuitGame()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

/*void UFMBMenuWidget::OnSelectCharacterClicked()
{
    if (GetWorld())
    {
        const auto GameMode = Cast<AFMBMenuGameModeBase>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->SetMenuState(EFMBMenuState::ChangeSkin);
        }
    }
}*/