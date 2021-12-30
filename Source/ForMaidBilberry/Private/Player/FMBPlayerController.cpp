// For Maid Bilberry Game. All Rights Recerved

#include "Player/FMBPlayerController.h"
#include "Components/FMBRespawnComponent.h"
#include "FMBGameModeBase.h"
#include "FMBGameInstance.h"
#include "UI/FMBKeyboardInfoWidget.h"

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
    InputComponent->BindAction("Mute", IE_Pressed, this, &AFMBPlayerController::OnVolumeMute);
    InputComponent->BindAction("Keyboard", IE_Pressed, this, &AFMBPlayerController::OnKeyboardInfoVisible);
}

void AFMBPlayerController::OnGamePause()
{
    if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

    GetWorld()->GetAuthGameMode()->SetPause(this);
}

void AFMBPlayerController::OnVolumeMute()
{
    if (!GetWorld()) return;

    const auto FMBGameInstance = Cast<UFMBGameInstance>(GetWorld()->GetGameInstance());
    if (!FMBGameInstance) return;

    FMBGameInstance->ToggleVolume();
}

void AFMBPlayerController::OnKeyboardInfoVisible()
{
    IsKeyboardInfoVisible = IsKeyboardInfoVisible ? false : true;
}