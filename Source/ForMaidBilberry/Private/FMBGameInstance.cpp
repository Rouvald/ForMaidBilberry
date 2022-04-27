// For Maid Bilberry Game. All Rights Reserved

#include "FMBGameInstance.h"
#include "Sound/FMBSoundFuncLib.h"

void UFMBGameInstance::ToggleVolume() const
{
    UFMBSoundFuncLib::ToggleSoundClassVolume(MasterSoundClass);
}