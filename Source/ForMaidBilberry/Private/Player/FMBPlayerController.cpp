// For Maid Bilberry Game. All Rights Recerved


#include "Player/FMBPlayerController.h"
#include "Components/FMBRespawnComponent.h"

AFMBPlayerController::AFMBPlayerController()
{
    RespawnComponent = CreateDefaultSubobject<UFMBRespawnComponent>("RespawnComponent");
}