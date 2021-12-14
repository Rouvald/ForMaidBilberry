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

DEFINE_LOG_CATEGORY_STATIC(LogAFMBGameModeBase, All, All)

constexpr static int32 MinRoundTimeForRespawn = 10;

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
    //CreateTeamsInfo();

    //CurrentRound = 1;
    StartRound();

    SetMatchState(EFMBMatchState::InProgress);
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

    for (int32 i = 0; i < GameData.PlayerNum - 1; ++i)
    {
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        const auto FMBAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
        RestartPlayer(FMBAIController);
    }
}

void AFMBGameModeBase::StartRound()
{
    if (GameData.InfinityGame) return;

    GameplayTimeCountDown = GameData.GameplayTime;
    GetWorld()->GetTimerManager().SetTimer(RoundTimerHandle, this, &AFMBGameModeBase::GameTimerUpdate, 1.0f, true);
}

void AFMBGameModeBase::GameTimerUpdate()
{
    UE_LOG(LogAFMBGameModeBase, Display, TEXT("Time: %i-%i"), GameplayTimeCountDown, GameData.GameplayTime);
    if (--GameplayTimeCountDown == 0)
    {
        GetWorld()->GetTimerManager().ClearTimer(RoundTimerHandle);

        GameOver();
        /*if (CurrentRound + 1 <= GameData.RoundsNum)
        {
            ++CurrentRound;
            ResetPlayers();
            StartRound();
        }
        else
        {
            UE_LOG(LogAFMBGameModeBase, Display, TEXT("========GAME OVER========"));
            LogPlayerInfo();
        }*/
    }
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
    //SetTeamSkeletalMesh(Controller);
}

void AFMBGameModeBase::Killed(AController* PlayerController, bool IsKill)
{
    const auto PlayerState = PlayerController ? Cast<AFMBPlayerState>(PlayerController->PlayerState) : nullptr;

    if (PlayerState)
    {
        if (IsKill)
        {
            PlayerState->AddKill();
            InfinityGameOverCondition();
        }
        else
        {
            PlayerState->AddDeath();
            StartRespawn(PlayerController);
        }
    }
}

void AFMBGameModeBase::InfinityGameOverCondition()
{
    if (!GetWorld()) return;
    if (!GameData.InfinityGame) return;

    const auto PlayerController = Cast<AFMBPlayerController>(GetWorld()->GetFirstPlayerController());
    if (!PlayerController) return;

    const auto PlayerState = Cast<AFMBPlayerState>(PlayerController->PlayerState);
    if (!PlayerState) return;

    if (GameData.PlayerNum > 1 && GameData.PlayerNum - 1 - PlayerState->GetKillsNum() == 0)
    {
        GetWorld()->GetTimerManager().SetTimer(GameOverTimerHandle, this, &AFMBGameModeBase::GameOver, 5.0f, false);
    }
}

void AFMBGameModeBase::LogPlayerInfo()
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
    const auto IsRespawnAvailable = GameData.InfinityGame || GameplayTimeCountDown > MinRoundTimeForRespawn + GameData.RespawnTime;
    if (!IsRespawnAvailable) return;

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
    UE_LOG(LogAFMBGameModeBase, Display, TEXT("========GAME OVER========"));
    LogPlayerInfo();

    for (const auto Pawn : TActorRange<APawn>(GetWorld()))
    {
        if (Pawn)
        {
            Pawn->TurnOff();
            Pawn->DisableInput(nullptr);
        }
    }
    SetMatchState(EFMBMatchState::GameOver);
}

void AFMBGameModeBase::SetMatchState( EFMBMatchState State)
{
    if (MatchState == State) return;

    MatchState = State;
    OnMatchStateChange.Broadcast(MatchState);
}

bool AFMBGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
    const auto IsPauseSet= Super::SetPause(PC, CanUnpauseDelegate);
    if(IsPauseSet)
    {
        SetMatchState(EFMBMatchState::Pause);
    }
    return IsPauseSet;
}

bool AFMBGameModeBase::ClearPause()
{
    const auto IsPauseClear = Super::ClearPause();
    if(IsPauseClear)
    {
        SetMatchState(EFMBMatchState::InProgress);
    }
    return IsPauseClear;
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
