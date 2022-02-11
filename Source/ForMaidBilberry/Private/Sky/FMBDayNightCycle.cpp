// For Maid Bilberry Game. All Rights Reserved

#include "Sky/FMBDayNightCycle.h"
#include "FMBGameModeBase.h"
#include "Misc/OutputDeviceNull.h"
#include "Engine/DirectionalLight.h"
#include "Components/LightComponent.h"

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
        FMBGameMode->OnDayNightChange.AddUObject(this, &AFMBDayNightCycle::InterpSunIntensity);
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

void AFMBDayNightCycle::SetSkyDefaultRotation()
{
    if (!FMBGameMode) return;

    const float DefaultPitch{FMBGameMode->GetDefaultSunRotation()};

    if (LightSource)
    {
        LightSource->SetActorRotation(FRotator(DefaultPitch, 0.0f, 0.0f));
        DayLightIntensity = LightSource->GetLightComponent()->Intensity;
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
        SunRotation();
        // UE_LOG(LogFMBDayNightCycle, Display, TEXT("%f"), LightSource->GetActorRotation().Pitch);
    }
    if (SkySphere)
    {
        FOutputDeviceNull OutputDevice;
        SkySphere->CallFunctionByNameWithArguments(TEXT("UpdateSunDirection"), OutputDevice, nullptr, true);
    }
}

void AFMBDayNightCycle::SunRotation() const
{
    if (!LightSource) return;
    /*
    float LightPitch = LightSource->GetActorRotation().Pitch + SunPitchRotation;
    LightPitch = FMath::FInterpTo(LightPitch, LightPitch+, GetWorld()->GetDeltaSeconds(), 120.0f);
    LightSource->SetActorRelativeRotation(FRotator{LightPitch, 0.0f, 0.0f});
    */
    LightSource->AddActorLocalRotation(FRotator(SunPitchRotation, 0.0f, 0.0f));
    // UE_LOG(LogFMBDayNightCycle, Display, TEXT("%s"), *LightSource->GetActorRotation().ToString());
}

void AFMBDayNightCycle::InterpSunIntensity(const EDayTime NewDayTime)
{
    if (!GetWorld() || !LightSource) return;
    float CurrentLightIntensity{0.0f};
    switch (NewDayTime)
    {
    case EDayTime::EDT_Day:
        CurrentLightIntensity = FMath::FInterpTo(DayLightIntensity, NightLightIntensity, GetWorld()->GetDeltaSeconds(), 15.0f);
        break;
    case EDayTime::EDT_Night:
        CurrentLightIntensity = FMath::FInterpTo(NightLightIntensity, DayLightIntensity, GetWorld()->GetDeltaSeconds(), 15.0f);
        break;
    default:
        break;
    }
    // UE_LOG(LogFMBDayNightCycle, Display, TEXT("Intensity: %f"), CurrentLightIntensity);
    // UE_LOG(LogFMBDayNightCycle, Display, TEXT("Pitch: %f"), LightSource->GetActorRotation().Pitch);
    LightSource->GetLightComponent()->SetIntensity(CurrentLightIntensity);
}

EDayTime AFMBDayNightCycle::GetDayTime() const
{
    if (!FMBGameMode) return EDayTime::EDT_Day;
    return FMBGameMode->GetDayTime();
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
