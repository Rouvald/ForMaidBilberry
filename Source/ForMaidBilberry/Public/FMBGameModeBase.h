// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FMBCoreTypes.h"
#include "FMBGameModeBase.generated.h"

class AAIController;

UCLASS()
class FORMAIDBILBERRY_API AFMBGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    AFMBGameModeBase();

    virtual void StartPlay() override;

    UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

protected:
    UPROPERTY(EditDefaultsOnly, Category="Game")
    TSubclassOf<AAIController> AIControllerClass;

    UPROPERTY(EditDefaultsOnly, Category="Game")
    TSubclassOf<APawn> AIPawnClass;

    UPROPERTY(EditDefaultsOnly, Category="Game")
    FGameData GameData;

private:
    int32 CurrentRound = 1;
    int32 RoundCountDown = 0;
    FTimerHandle RoundTimerHandle;
    
    void SpawnBots();

    void StartRound();
    void GameTimerUpdate();

    void ResetPlayers();
    void ResetOnePlayer(AController* Controller);

    void CreateTeamsInfo();
    USkeletalMesh* DetermineColorByTeamID(int32 TeamID);
    void SetTeamColor(AController* Controller);    
};
