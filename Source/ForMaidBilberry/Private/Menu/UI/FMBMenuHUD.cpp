// For Maid Bilberry Game. All Rights Recerved


#include "Menu/UI/FMBMenuHUD.h"
#include "Blueprint/UserWidget.h"
#include "Menu/FMBMenuGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBMenuHUD, All, All)

void AFMBMenuHUD::BeginPlay()
{
    Super::BeginPlay();

    MenuWidgets.Add(EFMBMenuState::MainMenu, CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass));
    MenuWidgets.Add(EFMBMenuState::SelectLevel, CreateWidget<UUserWidget>(GetWorld(), SelectLevelWidgetClass));
    //MenuWidgets.Add(EFMBMenuState::ChangeSkin, CreateWidget<UUserWidget>(GetWorld(), SelectCharacterWidgetClass));
    for (const auto MenuWidgetPair : MenuWidgets)
    {
        const auto MenuWidget = MenuWidgetPair.Value;
        if (!MenuWidget) continue;

        MenuWidget->AddToViewport();
        MenuWidget->SetVisibility(ESlateVisibility::Hidden);
    }
    if(!CurrentWidget)
    {
        CurrentWidget = MenuWidgets[EFMBMenuState::MainMenu];
    }
    
    if (GetWorld())
    {
        const auto GameMode = Cast<AFMBMenuGameModeBase>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->OnMenuStateChange.AddUObject(this, &AFMBMenuHUD::OnMenuStateChanged);
        }
    }
}

void AFMBMenuHUD::OnMenuStateChanged(EFMBMenuState State)
{
    if(CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
    }
    if(MenuWidgets.Contains(State))
    {
        CurrentWidget = MenuWidgets[State];
    }
    if(CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Visible);
    }
    UE_LOG(LogFMBMenuHUD, Display, TEXT("Menu State: %s"), *UEnum::GetValueAsString(State));
}

/*void AFMBMenuHUD::SetCurrentWidgetVisibility(bool IsVisibility) const
{
    if(CurrentWidget)
    {
        CurrentWidget->SetVisibility(IsVisibility ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}*/