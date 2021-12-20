// For Maid Bilberry Game. All Rights Recerved

#include "Menu/FMBMenuPlayerController.h"

void AFMBMenuPlayerController::BeginPlay()
{
    Super::BeginPlay();

    SetInputMode(FInputModeUIOnly());
    bShowMouseCursor = true;
}
