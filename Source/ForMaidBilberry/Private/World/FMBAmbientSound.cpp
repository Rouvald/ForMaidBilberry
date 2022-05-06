// For Maid Bilberry Game. All Rights Reserved

#include "World/FMBAmbientSound.h"
#include "FMBGameModeBase.h"
#include "Components/AudioComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBAmbientSound, All, All)

AFMBAmbientSound::AFMBAmbientSound()
{
    PrimaryActorTick.bCanEverTick = false;

    RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
    SetRootComponent(RootSceneComponent);

    DaySound = CreateDefaultSubobject<UAudioComponent>(TEXT("DaySound"));
    DaySound->SetupAttachment(GetRootComponent());

    NightSound = CreateDefaultSubobject<UAudioComponent>(TEXT("NightSound"));
    NightSound->SetupAttachment(GetRootComponent());
}

void AFMBAmbientSound::BeginPlay()
{
    Super::BeginPlay();

    FMBGameMode = GetFMBGameMode();
    if (FMBGameMode)
    {
        FMBGameMode->OnDayNightChange.AddUObject(this, &AFMBAmbientSound::SetAmbientSound);
    }
    else
    {
        UE_LOG(LogFMBAmbientSound, Warning, TEXT("FMBGameMode == nullptr"));
    }
}

void AFMBAmbientSound::SetAmbientSound(const EDayTime NewDayTime) const
{
    // UE_LOG(LogFMBAmbientSound, Display, TEXT("%s"), *UEnum::GetValueAsString(NewDayTime));
    switch (NewDayTime)
    {
    case EDayTime::EDT_Day:
        NightSound->FadeOut(FadeOutVector.X, FadeOutVector.Y);
        DaySound->FadeIn(FadeInVector.X, FadeInVector.Y);
        break;
    case EDayTime::EDT_Night:
        DaySound->FadeOut(FadeOutVector.X, FadeOutVector.Y);
        NightSound->FadeIn(FadeInVector.X, FadeInVector.Y);
        break;
    default:
        break;
    }
}

AFMBGameModeBase* AFMBAmbientSound::GetFMBGameMode() const
{
    if (!GetWorld()) return nullptr;
    return Cast<AFMBGameModeBase>(GetWorld()->GetAuthGameMode());
}