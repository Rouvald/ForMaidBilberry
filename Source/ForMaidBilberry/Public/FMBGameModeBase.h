// For Maid Bilberry Game. All Rights Reserved

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
    FOnChangeSunRotationSignature OnChangeSunRotation;
    FOnDayNightChangeSignature OnDayNightChange;

    virtual void StartPlay() override;

    virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

    virtual AActor* FindPlayerStart_Implementation(AController* Player, const FString& IncomingName) override;

    virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
    virtual bool ClearPause() override;

    void PlayerKiller(AController* KillerController, AController* VictimController);
    void BotKiller(AController* VictimController);

    FORCEINLINE FGameData GetGameData() const { return GameData; }
    FORCEINLINE float GetGameTimer() const { return CurrentDaySecondTime; }

    void RespawnRequest(AController* Controller);

    // bool GetIsDefaultDay() const { return GameData.bIsDefaultDay; }
    // FORCEINLINE float GetTurnRatePitch() const { return GameData.TurnRatePitch; }
    FORCEINLINE float GetDefaultSunRotation() const { return DefaultDayTimeMap[GameData.DefaultDayTime]; }

    FORCEINLINE EDayTime GetDayTime() const { return CurrentDayTime; }

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<AAIController> AIControllerClass;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<APawn> AIPawnClass;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FGameData GameData;

    /*UPROPERTY(EditDefaultsOnly, Category = "Game")
    FName FMBDefaultPlayerName{"Player"};*/

    UPROPERTY(VisibleAnywhere, Category = "Game", meta = (ToolTip = "This tag must have at least 1 PlayerStart in the Level."))
    FName PlayerStartTagName{"Player"};

private:
    TMap<EDayTime, float> DefaultDayTimeMap;
    float CurrentDaySecondTime{0};
    // float MaxDaySecondsTime{0.0f};
    float DayTimeModifier{0.0f};
    float SunRotationTimeCounter{0.0f};

    FTimerHandle DayTimerHandle;
    FTimerHandle GameOverConditionTimerHandle;

    EGameState MatchState{EGameState::EGS_WaitingToStart};

    EDayTime CurrentDayTime{EDayTime::EDT_Max};

    void SpawnBots();
    void SetDefaultPlayerName();
    void SetStartUpDayTime();
    void SetDayTime(const EDayTime NewDayTime);

    void DayTimerUpdate();
    void UpdateIsDayTime();

    void ResetPlayers();
    void ResetOnePlayer(AController* Controller);
    void StartRespawn(AController* Controller);

    void LogPlayerInfo() const;

    void GameOver();
    void GameOverCondition();

    void SetMatchState(EGameState State);
};
