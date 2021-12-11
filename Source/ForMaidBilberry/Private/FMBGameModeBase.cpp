// For Maid Bilberry Game. All Rights Recerved


#include "FMBGameModeBase.h"
#include "Player/FMBBaseCharacter.h"
#include "Player/FMBPlayerController.h"
#include "UI/FMBGameHUD.h"
#include "AIController.h"
#include "Player/FMBPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(AFMBGameModeBaseLog, All, All)

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
    CreateTeamsInfo();

    CurrentRound = 1;
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
    RoundCountDown = GameData.RoundsTime;
    GetWorld()->GetTimerManager().SetTimer(RoundTimerHandle, this, &AFMBGameModeBase::GameTimerUpdate, 1.0f, true);
}

void AFMBGameModeBase::GameTimerUpdate()
{
    UE_LOG(AFMBGameModeBaseLog, Display, TEXT("Time: %i-%i, Round: %i-%i"), RoundCountDown, GameData.RoundsTime, CurrentRound,
        GameData.RoundsNum);
    if (--RoundCountDown == 0)
    {
        GetWorld()->GetTimerManager().ClearTimer(RoundTimerHandle);

        if (CurrentRound + 1 <= GameData.RoundsNum)
        {
            ++CurrentRound;
            ResetPlayers();
            StartRound();
        }
        else
        {
            UE_LOG(AFMBGameModeBaseLog, Display, TEXT("========GAME OVER========"));
        }
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
    SetTeamColor(Controller);
}

void AFMBGameModeBase::CreateTeamsInfo()
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
            PlayerState->SetTeamColor(DetermineColorByTeamID(TeamID));
            SetTeamColor(Controller);
        }
        else
        {
            TeamID = 2;
            PlayerState->SetTeamID(TeamID);
            PlayerState->SetTeamColor(DetermineColorByTeamID(TeamID));
            SetTeamColor(Controller);
        }
    }
}

USkeletalMesh* AFMBGameModeBase::DetermineColorByTeamID(int32 TeamID)
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
}
