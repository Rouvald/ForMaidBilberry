// For Maid Bilberry Game. All Rights Recerved

#include "UI/FMBPlayerStatRowWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UFMBPlayerStatRowWidget::SetPlayerName(const FText& Text) const
{
    if (!PlayerNameText) return;
    PlayerNameText->SetText(Text);
}

void UFMBPlayerStatRowWidget::SetKillsNum(const FText& Text) const
{
    if (!KillsNumText) return;
    KillsNumText->SetText(Text);
}

void UFMBPlayerStatRowWidget::SetDeathsNum(const FText& Text) const
{
    if (!DeathsNumText) return;
    DeathsNumText->SetText(Text);
}

void UFMBPlayerStatRowWidget::SetBackGroundImageVisibility(bool IsVisible) const
{
    if (!BackgroundImage) return;
    BackgroundImage->SetVisibility(IsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}