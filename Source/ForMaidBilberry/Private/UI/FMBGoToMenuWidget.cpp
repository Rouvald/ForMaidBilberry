// For Maid Bilberry Game. All Rights Recerved


#include "UI/FMBGoToMenuWidget.h"
#include "FMBGameInstance.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBGoToMenuWidget, All, All)

void UFMBGoToMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if(GoToMainMenuButton)
    {
        GoToMainMenuButton->OnClicked.AddDynamic(this, &UFMBGoToMenuWidget::OnGoToMenuButtonClicked);
    }
}

void UFMBGoToMenuWidget::OnGoToMenuButtonClicked()
{
    if(!GetWorld()) return;

    const auto FMBGameInstance = GetWorld()->GetGameInstance<UFMBGameInstance>();
    if(!FMBGameInstance) return;

    if(FMBGameInstance->GetMenuLevelName().IsNone())
    {
        UE_LOG(LogFMBGoToMenuWidget, Error, TEXT("Menu level name is NONE"));
        return;
    }
    UGameplayStatics::OpenLevel(this, FMBGameInstance->GetMenuLevelName());
}