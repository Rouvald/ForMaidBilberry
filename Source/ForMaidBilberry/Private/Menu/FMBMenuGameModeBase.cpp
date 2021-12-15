// For Maid Bilberry Game. All Rights Recerved


#include "Menu/FMBMenuGameModeBase.h"
#include "FMBMenuHUD.h"
#include "Menu/FMBMenuPlayerController.h"

AFMBMenuGameModeBase::AFMBMenuGameModeBase()
{
    PlayerControllerClass = AFMBMenuPlayerController::StaticClass();
    HUDClass = AFMBMenuHUD::StaticClass();
}