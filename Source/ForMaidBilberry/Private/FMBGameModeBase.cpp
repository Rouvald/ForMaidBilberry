// For Maid Bilberry Game. All Rights Recerved


#include "FMBGameModeBase.h"
#include "Player/FMBBaseCharacter.h"
#include "Player/FMBPlayerController.h"
#include "UI/FMBGameHUD.h"

AFMBGameModeBase::AFMBGameModeBase()
{
    DefaultPawnClass = AFMBBaseCharacter::StaticClass();
    PlayerControllerClass = AFMBPlayerController::StaticClass();
    HUDClass = AFMBGameHUD::StaticClass();
}