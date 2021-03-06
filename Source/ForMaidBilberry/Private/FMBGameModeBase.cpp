// For Maid Bilberry Game. All Rights Reserved

#include "FMBGameModeBase.h"
#include "Player/FMBPlayerCharacter.h"
#include "Player/FMBPlayerController.h"
#include "UI/FMBGameHUD.h"
#include "AIController.h"
#include "EngineUtils.h"
#include "FMBRespawnComponent.h"
#include "FMBUtils.h"
#include "GameFramework/GameMode.h"
#include "Player/FMBPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBGameModeBase, All, All)

constexpr static float WholeDayInSeconds{1440.0f};
constexpr static float MinuteToSecond{60.0f};
constexpr static float DayTimeRate{0.01f};
constexpr static float UpdateSunRotationCounter{0.3f};

AFMBGameModeBase::AFMBGameModeBase()
{
    DefaultPawnClass = AFMBPlayerCharacter::StaticClass();
    PlayerControllerClass = AFMBPlayerController::StaticClass();
    HUDClass = AFMBGameHUD::StaticClass();
    PlayerStateClass = AFMBPlayerState::StaticClass();

    /* Add default sun position in map */
    DefaultDayTimeMap.Add(EDayTime::EDT_Day, -90.0f);
    // DefaultDayTimeMap.Add(EDayTime::EDT_Morning, 0.1f);
    // DefaultDayTimeMap.Add(EDayTime::EDT_Evening, -0.1f);
    DefaultDayTimeMap.Add(EDayTime::EDT_Night, 90.0f);
}

void AFMBGameModeBase::StartPlay()
{
    Super::StartPlay();

    SpawnBots();
    SetDefaultPlayerName();
    SetStartUpDayTime();
    SetMatchState(EGameState::EGS_InProgress);
}

UClass* AFMBGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    if (InController && InController->IsA<AAIController>())
    {
        return AIPawnClass;
    }
    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void AFMBGameModeBase::SpawnBots()
{
    if (!GetWorld()) return;

    for (int32 i = 0; i < (GameData.PlayerNum - 1); ++i)
    {
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        const auto FMBAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
        RestartPlayer(FMBAIController);
    }
}

void AFMBGameModeBase::SetStartUpDayTime()
{
    DayTimeModifier = WholeDayInSeconds / (GameData.MaxDayMinuteTime * MinuteToSecond);

    switch (GameData.DefaultDayTime)
    {
    case EDayTime::EDT_Day:
        CurrentDaySecondTime = WholeDayInSeconds / 2;
        break;
    /*case EDayTime::EDT_Morning:
        CurrentDaySecondTime = WholeDayInSeconds / 4;
        break;
    case EDayTime::EDT_Evening:
        CurrentDaySecondTime = WholeDayInSeconds * 3 / 4;
        break;*/
    case EDayTime::EDT_Night:
        CurrentDaySecondTime = 0.0f;
        break;
    default /*case EDayTime::EDT_Max*/:
        UE_LOG(LogFMBGameModeBase, Warning, TEXT("Incorrect Day time."));
        break;
    }
    SetDayTime(GameData.DefaultDayTime);
    GetWorldTimerManager().SetTimer(DayTimerHandle, this, &AFMBGameModeBase::DayTimerUpdate, DayTimeRate, true);
}

void AFMBGameModeBase::DayTimerUpdate()
{
    const float CurrentDaySecondTimeTEMP = CurrentDaySecondTime + DayTimeRate * DayTimeModifier;
    CurrentDaySecondTime = FMath::Clamp(FMath::Fmod(CurrentDaySecondTimeTEMP, WholeDayInSeconds), 0.0f, WholeDayInSeconds);
    UpdateIsDayTime();
    if (SunRotationTimeCounter >= UpdateSunRotationCounter)
    {
        OnChangeSunRotation.Broadcast();
        SunRotationTimeCounter = 0.0f;
    }
    else
    {
        SunRotationTimeCounter += DayTimeRate * DayTimeModifier;
    }
}

void AFMBGameModeBase::UpdateIsDayTime()
{
    const float SixAM = WholeDayInSeconds / 4;
    const float SixPM = WholeDayInSeconds * 3 / 4;
    FMath::IsWithin(CurrentDaySecondTime, SixAM, SixPM) ? SetDayTime(EDayTime::EDT_Day) : SetDayTime(EDayTime::EDT_Night);
    // UE_LOG(LogFMBGameModeBase, Display, TEXT("%d ==== %f :::: %f"), CurrentDayTime, SixAM, SixPM);
}

void AFMBGameModeBase::SetDayTime(const EDayTime NewDayTime)
{
    if (CurrentDayTime == NewDayTime) return;

    // UE_LOG(LogFMBGameModeBase, Display, TEXT("%s"), *UEnum::GetValueAsString(NewDayTime));
    CurrentDayTime = NewDayTime;
    OnDayNightChange.Broadcast(NewDayTime);
}

void AFMBGameModeBase::ResetPlayers()
{
    if (!GetWorld()) return;

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        ResetOnePlayer(It->Get());
    }
}

void AFMBGameModeBase::ResetOnePlayer(AController* Controller)
{
    if (Controller && Controller->GetPawn())
    {
        Controller->GetPawn()->Reset();
    }
    RestartPlayer(Controller);
}

AActor* AFMBGameModeBase::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
{
    if (Player->IsPlayerController())
    {
        return Super::FindPlayerStart_Implementation(Player, PlayerStartTagName.ToString());
    }
    return Super::FindPlayerStart_Implementation(Player, IncomingName);
}

void AFMBGameModeBase::PlayerKiller(AController* KillerController, AController* VictimController)
{
    const auto KillerPlayerState = KillerController ? Cast<AFMBPlayerState>(KillerController->PlayerState) : nullptr;

    if (KillerPlayerState)
    {
        KillerPlayerState->AddKill();
        GetWorldTimerManager().SetTimer(
            GameOverConditionTimerHandle, this, &AFMBGameModeBase::GameOverCondition, GameData.RespawnTime + 1.0f);
    }
    if (VictimController && GameData.CanBotsRespawn)
    {
        StartRespawn(VictimController);
    }
}

void AFMBGameModeBase::BotKiller(AController* VictimController)
{
    if (const auto VictimPlayerState = VictimController ? Cast<AFMBPlayerState>(VictimController->PlayerState) : nullptr)
    {
        VictimPlayerState->AddDeath();
        StartRespawn(VictimController);
    }
}

void AFMBGameModeBase::GameOverCondition()
{
    if (!GetWorld()) return;

    if (GetWorld()->GetNumControllers() == 1)
    {
        GameOver();
    }
}

void AFMBGameModeBase::LogPlayerInfo() const
{
    if (!GetWorld()) return;

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if (!Controller) continue;

        const auto PlayerState = Cast<AFMBPlayerState>(Controller->PlayerState);
        if (!PlayerState) continue;

        PlayerState->LogInfo();
    }
}

void AFMBGameModeBase::StartRespawn(AController* Controller)
{
    /*const auto IsRespawnAvailable = GameData.IsInfinityGame || CurrentDaySecondTime > MinRoundTimeForRespawn + GameData.RespawnTime;
    if (!IsRespawnAvailable) return;*/

    const auto RespawnComponent = FMBUtils::GetFMBPlayerComponent<UFMBRespawnComponent>(Controller);
    if (!RespawnComponent) return;

    RespawnComponent->Respawn(GameData.RespawnTime);
}

void AFMBGameModeBase::RespawnRequest(AController* Controller)
{
    ResetOnePlayer(Controller);
}

void AFMBGameModeBase::GameOver()
{
    UE_LOG(LogFMBGameModeBase, Display, TEXT("========GAME OVER========"));
    LogPlayerInfo();

    for (const auto Pawn : TActorRange<APawn>(GetWorld()))
    {
        if (Pawn)
        {
            Pawn->TurnOff();
            Pawn->DisableInput(nullptr);
        }
    }
    SetMatchState(EGameState::EGS_GameOver);
}

void AFMBGameModeBase::SetMatchState(EGameState State)
{
    if (MatchState == State) return;

    MatchState = State;
    OnMatchStateChange.Broadcast(MatchState);
}

bool AFMBGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
    const auto IsPauseSet = Super::SetPause(PC, CanUnpauseDelegate);
    if (IsPauseSet)
    {
        SetMatchState(EGameState::EGS_Pause);
    }
    return IsPauseSet;
}

bool AFMBGameModeBase::ClearPause()
{
    const auto IsPauseClear = Super::ClearPause();
    if (IsPauseClear)
    {
        SetMatchState(EGameState::EGS_InProgress);
    }
    return IsPauseClear;
}

void AFMBGameModeBase::SetDefaultPlayerName()
{
    if (!GetWorld()) return;

    const auto PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController) return;

    const auto PlayerState = Cast<AFMBPlayerState>(PlayerController->PlayerState);
    if (!PlayerState) return;

    auto PlayerName{PlayerState->GetPlayerName()};
    const auto Index{PlayerName.Find("-")};
    PlayerName = PlayerName.LeftChop(PlayerName.Len() - Index);
    PlayerState->SetPlayerName(PlayerName);
}

/*void AFMBGameModeBase::CreateTeamsInfo()
{
    if (!GetWorld()) return;

    int32 TeamID = 1;
    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if (!Controller) continue;

        const auto PlayerState = Cast<AFMBPlayerState>(Controller->PlayerState);
        if (!PlayerState) continue;

        if(Cast<AFMBPlayerController>(Controller))
        {
            PlayerState->SetTeamID(TeamID);
            //PlayerState->SetTeamSkeletalMesh(DetermineColorByTeamID(TeamID));
            //SetTeamSkeletalMesh(Controller);
        }
        else
        {
            TeamID = 2;
            PlayerState->SetTeamID(TeamID);
            //PlayerState->SetTeamSkeletalMesh(DetermineColorByTeamID(TeamID));
            //SetTeamColor(Controller);
        }
    }
}*/

/*USkeletalMesh* AFMBGameModeBase::DetermineSkeletalMeshByTeamID(int32 TeamID)
{
    if (TeamID - 1 < GameData.TeamSkeletalMeshes.Num())
    {
        return GameData.TeamSkeletalMeshes[TeamID - 1];
    }
    UE_LOG(AFMBGameModeBaseLog, Warning, TEXT("Not skeletal mesh for Team %i"), TeamID);
    return GameData.DefaultTeamSkeletalMesh;
}

void AFMBGameModeBase::SetTeamColor(AController* Controller)
{
    if (!Controller) return;

    const auto FMBCharacter = Cast<AFMBBaseCharacter>(Controller->GetPawn());
    if(!FMBCharacter) return;

    const auto PlayerState = Cast<AFMBPlayerState>(Controller->PlayerState);
    if(!PlayerState) return;

    FMBCharacter->SetTeamSkeletalMesh(PlayerState->GetTeamColor());
}*/
