// For Maid Bilberry Game. All Rights Recerved


#include "UI/FMBGameDataWidget.h"
#include "FMBGameModeBase.h"
#include "FMBPlayerState.h"

int32 UFMBGameDataWidget::GetKillsNum() const
{
    const auto PlayerState = GetPlayerState();
    return PlayerState ? PlayerState->GetKillsNum() : 0;
}

int32 UFMBGameDataWidget::GetGameplayTimeRemaining() const
{
    const auto GameMode = GetGameModeBase();
    return GameMode ? GameMode->GetRoundCountDown() : 0;
}

bool UFMBGameDataWidget::GetIsInfinityGame() const
{
    const auto GameMode = GetGameModeBase();
    return GameMode ? GameMode->GetGameData().InfinityGame : false;
}

AFMBGameModeBase* UFMBGameDataWidget::GetGameModeBase() const
{
    return GetWorld() ? Cast<AFMBGameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr;
}

AFMBPlayerState* UFMBGameDataWidget::GetPlayerState() const
{
    return GetOwningPlayer() ? Cast<AFMBPlayerState>(GetOwningPlayer()->PlayerState) : nullptr;
}
