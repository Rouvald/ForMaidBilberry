// For Maid Bilberry Game. All Rights Recerved

#include "UI/FMBGameHUD.h"
#include "Engine/Canvas.h"
#include "FMBGameModeBase.h"
#include "UI/FMBBaseWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBGameHUD, All, All)

void AFMBGameHUD::BeginPlay()
{
    Super::BeginPlay();

    GameWidgets.Add(EFMBMatchState::InProgress, CreateWidget<UFMBBaseWidget>(GetWorld(), PLayerHUDWidgetClass));
    GameWidgets.Add(EFMBMatchState::Pause, CreateWidget<UFMBBaseWidget>(GetWorld(), GamePauseWidgetClass));
    GameWidgets.Add(EFMBMatchState::GameOver, CreateWidget<UFMBBaseWidget>(GetWorld(), GameOverWidgetClass));

    for (const auto GameWidgetPair : GameWidgets)
    {
        const auto GameWidget = GameWidgetPair.Value;
        if (!GameWidget) continue;

        GameWidget->AddToViewport();
        GameWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    if (GetWorld())
    {
        const auto GameMode = Cast<AFMBGameModeBase>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->OnMatchStateChange.AddUObject(this, &AFMBGameHUD::OnMatchStateChange);
        }
    }
}

void AFMBGameHUD::OnMatchStateChange(EFMBMatchState State)
{
    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
    }
    if (GameWidgets.Contains(State))
    {
        CurrentWidget = GameWidgets[State];
    }
    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Visible);
        CurrentWidget->Show();
    }
    UE_LOG(LogFMBGameHUD, Display, TEXT("Match State: %s"), *UEnum::GetValueAsString(State));
}

void AFMBGameHUD::DrawHUD()
{
    // DrawTwoLine();
}

void AFMBGameHUD::DrawTwoLine()
{
    const TInterval<float> Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);

    const float HalfLineSize = 5.0f;
    const float LineTHickness = 2.0f;
    const FColor ColorLine = FColor::Purple;

    DrawLine(Center.Min - HalfLineSize, Center.Max - HalfLineSize, Center.Min + HalfLineSize, Center.Max + HalfLineSize, ColorLine,
        LineTHickness);
    DrawLine(Center.Min - HalfLineSize, Center.Max + HalfLineSize, Center.Min + HalfLineSize, Center.Max - HalfLineSize, ColorLine,
        LineTHickness);
}
