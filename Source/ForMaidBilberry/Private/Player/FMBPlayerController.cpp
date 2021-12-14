// For Maid Bilberry Game. All Rights Recerved


#include "Player/FMBPlayerController.h"
#include "Components/FMBRespawnComponent.h"
#include "FMBGameModeBase.h"

AFMBPlayerController::AFMBPlayerController()
{
    RespawnComponent = CreateDefaultSubobject<UFMBRespawnComponent>("RespawnComponent");
}

void AFMBPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (GetWorld())
    {
        const auto GameMode = Cast<AFMBGameModeBase>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->OnMatchStateChange.AddUObject(this, &AFMBPlayerController::OnMatchStateChange);
        }
    }
}

void AFMBPlayerController::OnMatchStateChange(EFMBMatchState State)
{
    if (State == EFMBMatchState::InProgress)
    {
        SetInputMode(FInputModeGameOnly());
        bShowMouseCursor = false;
    }
    else
    {
        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor = true;
    }
}

void AFMBPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    if (!InputComponent) return;

    InputComponent->BindAction("PauseGame", IE_Pressed, this, &AFMBPlayerController::OnGamePause);
}

void AFMBPlayerController::OnGamePause()
{
    if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

    GetWorld()->GetAuthGameMode()->SetPause(this);
}
