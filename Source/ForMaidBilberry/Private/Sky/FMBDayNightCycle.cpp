// For Maid Bilberry Game. All Rights Recerved

#include "Sky/FMBDayNightCycle.h"
#include "FMBGameInstance.h"
#include "FMBGameModeBase.h"
#include "Misc/OutputDeviceNull.h"
#include "Engine/DirectionalLight.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBDayNightCycle, All, All)

constexpr static float SunPitchRotation{0.1f};

AFMBDayNightCycle::AFMBDayNightCycle()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AFMBDayNightCycle::BeginPlay()
{
    Super::BeginPlay();

    FMBGameMode = FMBGetGameMode();
    if (FMBGameMode)
    {
        FMBGameMode->OnChangeSunRotation.AddUObject(this, &AFMBDayNightCycle::UpdateSunRotation);
        SetSkyDefaultRotation();
    }
    else
    {
        UE_LOG(LogFMBDayNightCycle, Warning, TEXT("DayNightCycle Actor: GameMode = nullptr"));
    }
}

/*
void AFMBDayNightCycle::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);


}
*/

void AFMBDayNightCycle::SetSkyDefaultRotation() const
{
    if (!FMBGameMode) return;

    const float DefaultPitch{FMBGameMode->GetDefaultSunRotation()};

    if (LightSource)
    {
        LightSource->SetActorRotation(FRotator(DefaultPitch, 0.0f, 0.0f));
    }
    if (SkySphere)
    {
        FOutputDeviceNull OutputDevice;
        SkySphere->CallFunctionByNameWithArguments(TEXT("UpdateSunDirection"), OutputDevice, nullptr, true);
    }
}

void AFMBDayNightCycle::UpdateSunRotation() const
{
    if (!FMBGameMode) return;

    if (LightSource)
    {
        LightSource->AddActorLocalRotation(FRotator(SunPitchRotation, 0.0f, 0.0f));
        // UE_LOG(LogFMBDayNightCycle, Display, TEXT("%f"), LightSource->GetActorRotation().Pitch);
    }
    if (SkySphere)
    {
        FOutputDeviceNull OutputDevice;
        SkySphere->CallFunctionByNameWithArguments(TEXT("UpdateSunDirection"), OutputDevice, nullptr, true);
    }
}

bool AFMBDayNightCycle::GetIsDayTime() const
{
    if (!FMBGameMode) return false;

    return FMBGameMode->GetIsDayTime();
}

AFMBGameModeBase* AFMBDayNightCycle::FMBGetGameMode() const
{
    if (!GetWorld()) return nullptr;
    return Cast<AFMBGameModeBase>(GetWorld()->GetAuthGameMode());
}

/*
UFMBGameInstance* AFMBDayNightCycle::FMBGetGameInstance() const
{
    if (!GetWorld()) return nullptr;
    return Cast<UFMBGameInstance>(GetGameInstance());
}
*/
