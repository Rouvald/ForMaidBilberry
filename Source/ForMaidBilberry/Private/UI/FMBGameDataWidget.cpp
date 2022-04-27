// For Maid Bilberry Game. All Rights Reserved

#include "UI/FMBGameDataWidget.h"
#include "FMBGameModeBase.h"
#include "FMBPlayerState.h"
#include "Kismet/KismetStringLibrary.h"

FText UFMBGameDataWidget::GetKillsNum() const
{
    const auto PlayerState{GetPlayerState()};
    if (!PlayerState) return FText{FText::FromString(TEXT("Error"))};

    const FString KillsNum{TEXT("Kills: ") + FString::FromInt(PlayerState->GetKillsNum())};
    return FText::FromString(KillsNum);
}

FText UFMBGameDataWidget::GetEnemiesNum() const
{
    const auto GameMode = GetGameModeBase();
    const auto PlayerState = GetPlayerState();
    if (!GetWorld() || !PlayerState || !GameMode) return FText{FText::FromString(TEXT("Error"))};

    const int32 CurrentEnemiesNum{GetWorld()->GetNumControllers() - 1};
    const FString EnemiesNum{TEXT("Left ") + FString::FromInt(CurrentEnemiesNum) + TEXT(" enemy")};
    return FText::FromString(EnemiesNum);
}

FText UFMBGameDataWidget::GetDayTime() const
{
    const auto GameMode{GetGameModeBase()};
    if (!GameMode) return FText{FText::FromString(TEXT("Error"))};

    const FText DayTime{FText::FromString(UKismetStringLibrary::TimeSecondsToString(GameMode->GetGameTimer()).Mid(0, 5))};
    return DayTime;
}

ESlateVisibility UFMBGameDataWidget::IsPlayerAlone() const
{
    const auto GameMode{GetGameModeBase()};
    if (!GetWorld() || !GameMode) return ESlateVisibility::Hidden;

    const int32 CurrentEnemiesNum{GetWorld()->GetNumControllers() - 1};
    return (GameMode->GetGameData().bIsWalkAlone && CurrentEnemiesNum == 0) ? ESlateVisibility::Hidden : ESlateVisibility::Visible;
}

AFMBGameModeBase* UFMBGameDataWidget::GetGameModeBase() const
{
    return GetWorld() ? Cast<AFMBGameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr;
}

AFMBPlayerState* UFMBGameDataWidget::GetPlayerState() const
{
    return GetOwningPlayer() ? Cast<AFMBPlayerState>(GetOwningPlayer()->PlayerState) : nullptr;
}
