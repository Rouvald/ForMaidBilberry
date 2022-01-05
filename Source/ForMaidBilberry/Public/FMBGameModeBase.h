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

    FOnMatchStateChangeSignature OnMatchStateChange;

    virtual void StartPlay() override;

    virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

    virtual AActor* FindPlayerStart_Implementation(AController* Player, const FString& IncomingName) override;

    virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
    virtual bool ClearPause() override;

    void PlayerKiller(AController* KillerController, AController* VictimController);
    void BotKiller(AController* VictimController);

    FGameData GetGameData() const { return GameData; }
    float GetGameTimer() const { return CurrentDayTime; }

    void RespawnRequest(AController* Controller);

    float GetDefaultTurnRatePitchSky() const { return DefaultTurnRatePitchSky; }
    float GetTurnRatePitch() const { return TurnRatePitch; }

    UFUNCTION(BlueprintCallable)
    bool GetDayTime() const { return DayTime; }
    void SetDayTime(const bool IsDay);

    void DayTimerUpdate(float Time);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<AAIController> AIControllerClass;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<APawn> AIPawnClass;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FGameData GameData;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FName FMBDefaultPlayerName = "Player";

    UPROPERTY(VisibleAnywhere, Category = "Game", meta = (ToolTip = "This tag must have at least 1 PlayerStart in the Level."))
    FName PlayerStartTagName = "Player";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sky")
    float DefaultTurnRatePitchSky = -90.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sky", meta = (ToolTip = "Value = 0.23 is minimum for DayNightCycle"))
    float TurnRatePitch = 0.3f;

private:
    // int32 CurrentRound = 1;
    float CurrentDayTime = 0;
    float StartupDayTime = 0;
    float MaxDayTime = 1440; // 12 minute in second
    FTimerHandle DayTimerHandle;
    FTimerHandle GameOverConditionTimerHandle;
    FTimerHandle GameOverTimerHandle;

    EFMBMatchState MatchState = EFMBMatchState::WaitingToStart;

    bool DayTime = true;

    void SpawnBots();

    void StartGameOverConditionTimer();
    void GameOverCondition();

    void ResetPlayers();
    void ResetOnePlayer(AController* Controller);

    // void CreateTeamsInfo();
    // USkeletalMesh* DetermineSkeletalMeshByTeamID(int32 TeamID);
    // void SetTeamSkeletalMesh(AController* Controller);

    void LogPlayerInfo();

    void StartRespawn(AController* Controller);

    void GameOver();

    void SetMatchState(EFMBMatchState State);

    void SetDefaultPlayerName() const;

    void SetStartUpDayTime();
};
