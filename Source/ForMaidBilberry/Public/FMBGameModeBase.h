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

    void Killed(AController* PlayerController, bool IsKill);

    FGameData GetGameData() const {return GameData;}
    int32 GetRoundCountDown() const {return GameplayTimeCountDown;}

    void RespawnRequest(AController* Controller);

protected:
    UPROPERTY(EditDefaultsOnly, Category="Game")
    TSubclassOf<AAIController> AIControllerClass;

    UPROPERTY(EditDefaultsOnly, Category="Game")
    TSubclassOf<APawn> AIPawnClass;

    UPROPERTY(EditDefaultsOnly, Category="Game")
    FGameData GameData;

private:
    //int32 CurrentRound = 1;
    int32 GameplayTimeCountDown = 0;
    FTimerHandle RoundTimerHandle;

    void SpawnBots();

    void StartRound();
    void GameTimerUpdate();

    void ResetPlayers();
    void ResetOnePlayer(AController* Controller);

    //void CreateTeamsInfo();
    //USkeletalMesh* DetermineSkeletalMeshByTeamID(int32 TeamID);
    //void SetTeamSkeletalMesh(AController* Controller);

    void LogPlayerInfo();

    void StartRespawn(AController* Controller);

    void GameOver();
};
