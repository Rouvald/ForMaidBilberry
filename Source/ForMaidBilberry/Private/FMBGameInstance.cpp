// For Maid Bilberry Game. All Rights Recerved

#include "FMBGameInstance.h"
#include "Sound/FMBSoundFuncLib.h"

void UFMBGameInstance::ToggleVolume() const
{
    UFMBSoundFuncLib::ToggleSoundClassVolume(MasterSoundClass);
}