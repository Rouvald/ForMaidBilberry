// For Maid Bilberry Game. All Rights Recerved

#include "Sound/FMBSoundFuncLib.h"
#include "Sound/SoundClass.h"

void UFMBSoundFuncLib::SetSoundClassVolume(USoundClass* SoundClass, const float Volume)
{
    if (!SoundClass) return;

    SoundClass->Properties.Volume = FMath::Clamp(Volume, 0.0f, 1.0f);
}

void UFMBSoundFuncLib::ToggleSoundClassVolume(USoundClass* SoundClass)
{
    if (!SoundClass) return;

    const auto NextVolume = SoundClass->Properties.Volume ? 0.0f : 1.0f;
    SetSoundClassVolume(SoundClass, NextVolume);
}