// For Maid Bilberry Game. All Rights Reserved

#include "UI/FMBGameOverWidget.h"
#include "FMBGameModeBase.h"
#include "FMBPlayerStatRowWidget.h"
#include "Player/FMBPlayerState.h"
#include "FMBUtils.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"

void UFMBGameOverWidget::NativeOnInitialized()
{
    if (GetWorld())
    {
        if (const auto GameMode = Cast<AFMBGameModeBase>(GetWorld()->GetAuthGameMode()))
        {
            GameMode->OnMatchStateChange.AddUObject(this, &UFMBGameOverWidget::OnMatchStateChange);
        }
    }
    if (ResetLevelButton)
    {
        ResetLevelButton->OnClicked.AddDynamic(this, &UFMBGameOverWidget::OnResetLevel);
    }
}

void UFMBGameOverWidget::OnMatchStateChange(EGameState State)
{
    if (State == EGameState::EGS_GameOver)
    {
        UpdatePlayerStat();
    }
}

void UFMBGameOverWidget::UpdatePlayerStat()
{
    if (!GetWorld() || !PlayerStatBox) return;

    PlayerStatBox->ClearChildren();

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller{It->Get()};
        if (!Controller || !Controller->IsPlayerController()) continue;

        const auto PlayerState{Cast<AFMBPlayerState>(Controller->PlayerState)};
        if (!PlayerState) continue;

        const auto PlayerStatRow = CreateWidget<UFMBPlayerStatRowWidget>(GetWorld(), PlayerStatRowWidgetClass);
        if (!PlayerStatRow) continue;

        PlayerStatRow->SetPlayerName(FText::FromString(PlayerState->GetPlayerName()));
        PlayerStatRow->SetKillsNum(FMBUtils::TextFromInt(PlayerState->GetKillsNum()));
        PlayerStatRow->SetDeathsNum(FMBUtils::TextFromInt(PlayerState->GetDeathsNum()));
        PlayerStatRow->SetBackGroundImageVisibility(Controller->IsPlayerController());

        PlayerStatBox->AddChild(PlayerStatRow);
    }
}

void UFMBGameOverWidget::OnResetLevel()
{
    const auto ResetLevelName = UGameplayStatics::GetCurrentLevelName(this);
    UGameplayStatics::OpenLevel(this, FName(ResetLevelName));
}