// For Maid Bilberry Game. All Rights Recerved


#include "FMBGameModeBase.h"
#include "Player/FMBBaseCharacter.h"
#include "Player/FMBPlayerController.h"
#include "UI/FMBGameHUD.h"
#include "AIController.h"
#include "FMBRespawnComponent.h"
#include "FMBUtils.h"
#include "Player/FMBPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogAFMBGameModeBase, All, All)

constexpr static int32 MinRoundTimeForRespawn =10;

AFMBGameModeBase::AFMBGameModeBase()
{
    DefaultPawnClass = AFMBBaseCharacter::StaticClass();
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

        UE_LOG(LogAFMBGameModeBase, Display, TEXT("========GAME OVER========"));
        LogPlayerInfo();
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
        }
        else
        {
            PlayerState->AddDeath();
            StartRespawn(PlayerController);
        }
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
    const auto IsRespawnAvailable = GameplayTimeCountDown > MinRoundTimeForRespawn + GameData.RespawnTime;
    if(!IsRespawnAvailable) return;
    
    const auto RespawnComponent = FMBUtils::GetFMBPlayerComponent<UFMBRespawnComponent>(Controller);
    if (!RespawnComponent) return;

    RespawnComponent->Respawn(GameData.RespawnTime);
}

void AFMBGameModeBase::RespawnRequest(AController* Controller)
{
    ResetOnePlayer(Controller);
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
