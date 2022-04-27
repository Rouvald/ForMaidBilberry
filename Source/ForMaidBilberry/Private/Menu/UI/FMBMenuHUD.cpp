// For Maid Bilberry Game. All Rights Reserved

#include "Menu/UI/FMBMenuHUD.h"
#include "Blueprint/UserWidget.h"
#include "Menu/FMBMenuGameModeBase.h"
#include "UI/FMBBaseWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBMenuHUD, All, All)

void AFMBMenuHUD::BeginPlay()
{
    Super::BeginPlay();

    MenuWidgets.Add(EMenuState::EMS_MainMenu, CreateWidget<UFMBBaseWidget>(GetWorld(), MenuWidgetClass));
    MenuWidgets.Add(EMenuState::EMS_SelectLevel, CreateWidget<UFMBBaseWidget>(GetWorld(), SelectLevelWidgetClass));
    // MenuWidgets.Add(EMenuState::EMS_ChangeSkin, CreateWidget<UUserWidget>(GetWorld(), SelectCharacterWidgetClass));
    for (const auto MenuWidgetPair : MenuWidgets)
    {
        const auto MenuWidget = MenuWidgetPair.Value;
        if (!MenuWidget) continue;

        MenuWidget->AddToViewport();
        MenuWidget->SetVisibility(ESlateVisibility::Hidden);
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

void AFMBMenuHUD::OnMenuStateChanged(EMenuState State)
{
    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
    }
    if (MenuWidgets.Contains(State))
    {
        CurrentWidget = MenuWidgets[State];
    }
    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Visible);
        CurrentWidget->Show();
    }
    // UE_LOG(LogFMBMenuHUD, Display, TEXT("Menu State: %s"), *UEnum::GetValueAsString(State));
}

/*void AFMBMenuHUD::SetCurrentWidgetVisibility(bool IsVisibility) const
{
    if(CurrentWidget)
    {
        CurrentWidget->SetVisibility(IsVisibility ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}*/