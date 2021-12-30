// For Maid Bilberry Game. All Rights Recerved

#include "Sky/FMBDayNightCycle.h"
#include "FMBGameModeBase.h"
#include "Misc/OutputDeviceNull.h"
#include "Engine/DirectionalLight.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBDayNightCycle, All, All)

AFMBDayNightCycle::AFMBDayNightCycle()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AFMBDayNightCycle::BeginPlay()
{
    Super::BeginPlay();

    SetSkyDefaultRotation();
}

void AFMBDayNightCycle::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    const auto FMBGameMode = GetGameMode();
    if (FMBGameMode)
    {
        if (LightSource)
        {
            LightSource->AddActorLocalRotation(FRotator(DeltaTime * FMBGameMode->GetTurnRatePitch(), 0.0f, 0.0f));
            // FMBGameMode->CheckDayTime(LightSource->GetActorRotation().Pitch > 0.0f);
        }
        if (SkySphere)
        {
            FOutputDeviceNull OutputDevice;
            SkySphere->CallFunctionByNameWithArguments(TEXT("UpdateSunDirection"), OutputDevice, nullptr, true);
        }
    }
}

void AFMBDayNightCycle::SetSkyDefaultRotation()
{
    const auto FMBGameMode = GetGameMode();
    if (!FMBGameMode) return;

    if (LightSource)
    {
        LightSource->SetActorRotation(FRotator(FMBGameMode->GetDefaultTurnRatePitchSky(), 0.0f, 0.0f));
        // FMBGameMode->CheckDayTime(LightSource->GetActorRotation().Pitch > 0.0f);
    }
}

AFMBGameModeBase* AFMBDayNightCycle::GetGameMode()
{
    if (!GetWorld()) return nullptr;
    return Cast<AFMBGameModeBase>(GetWorld()->GetAuthGameMode());
}
