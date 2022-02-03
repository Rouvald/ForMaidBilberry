// For Maid Bilberry Game. All Rights Recerved

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

AFMBGameModeBase::AFMBGameModeBase()
{
    DefaultPawnClass = AFMBPlayerCharacter::StaticClass();
    PlayerControllerClass = AFMBPlayerController::StaticClass();
    HUDClass = AFMBGameHUD::StaticClass();
    PlayerStateClass = AFMBPlayerState::StaticClass();
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
    GameData.bIsDefaultDay ? CurrentDayTime = MaxDayTime / 2 : CurrentDayTime = 0.0f;
}

void AFMBGameModeBase::DayTimerUpdate(float Time)
{
    const float CurrentDayTimeTEMP = (CurrentDayTime + Time);
    CurrentDayTime = FMath::Fmod(CurrentDayTimeTEMP, MaxDayTime);
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
    /*const auto IsRespawnAvailable = GameData.IsInfinityGame || CurrentDayTime > MinRoundTimeForRespawn + GameData.RespawnTime;
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

void AFMBGameModeBase::SetDefaultPlayerName() const
{
    if (!GetWorld()) return;

    const auto PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController) return;

    const auto PlayerState = Cast<AFMBPlayerState>(PlayerController->PlayerState);
    if (!PlayerState) return;

    PlayerState->SetPlayerName(FMBDefaultPlayerName.ToString());
}

void AFMBGameModeBase::SetDayTime(const bool IsDay)
{
    if (DayTime == IsDay) return;

    DayTime = IsDay;
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
