// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "FMBGameModeBase.h"
#include "FMBPlayableGameMode.generated.h"

/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API AFMBPlayableGameMode : public AFMBGameModeBase
{
    GENERATED_BODY()
public:
    virtual void StartPlay() override;
};
