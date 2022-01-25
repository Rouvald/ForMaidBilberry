// For Maid Bilberry Game. All Rights Recerved

#include "Sky/FMBDayNightCycle.h"
#include "FMBGameModeBase.h"
#include "Misc/OutputDeviceNull.h"
#include "Engine/DirectionalLight.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBDayNightCycle, All, All)

constexpr static int32 OneSecond = 1;
constexpr static int32 PitchToSecondModifierDN = 8;

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
            FMBGameMode->SetDayTime(LightSource->GetActorRotation().Pitch > 0.0f);
        }
        if (SkySphere)
        {
            FOutputDeviceNull OutputDevice;
            SkySphere->CallFunctionByNameWithArguments(TEXT("UpdateSunDirection"), OutputDevice, nullptr, true);
        }
        UpdateDaytime(DeltaTime * FMBGameMode->GetTurnRatePitch());
    }
}

void AFMBDayNightCycle::SetSkyDefaultRotation() const
{
    const auto FMBGameMode = GetGameMode();
    if (!FMBGameMode) return;

    if (LightSource)
    {
        LightSource->SetActorRotation(FRotator(FMBGameMode->GetDefaultTurnRatePitchSky(), 0.0f, 0.0f));
        FMBGameMode->SetDayTime(LightSource->GetActorRotation().Pitch > 0.0f);
    }
}

void AFMBDayNightCycle::UpdateDaytime(const float TurnRateLight)
{
    const auto FMBGameMode = GetGameMode();
    if (!FMBGameMode) return;

    CountTime = TurnRateLight * (PitchToSecondModifierDN / 2);

    FMBGameMode->DayTimerUpdate(CountTime);
}

AFMBGameModeBase* AFMBDayNightCycle::GetGameMode() const
{
    if (!GetWorld()) return nullptr;
    return Cast<AFMBGameModeBase>(GetWorld()->GetAuthGameMode());
}
