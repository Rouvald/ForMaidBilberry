// For Maid Bilberry Game. All Rights Reserved

#include "GameModes/FMBPlayableGameMode.h"
#include "FMBGameInstance.h"

void AFMBPlayableGameMode::StartPlay()
{
    if (GetWorld())
    {
        const auto GameInstance = GetWorld()->GetGameInstance<UFMBGameInstance>();

        if (GameInstance)
        {
            GameData = GameInstance->GetStartLevel().GameData;
        }
    }
    if (GameData.bIsWalkAlone)
    {
        GameData.PlayerNum = 1;
    }
    Super::StartPlay();
}
