// For Maid Bilberry Game. All Rights Reserved

#include "Player/FMBPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogAFMBPlayerState, All, All)

void AFMBPlayerState::LogInfo()
{
    UE_LOG(LogAFMBPlayerState, Display, TEXT("Kills: %i, Deaths: %i"), KillsNum, DeathsNum);
}