// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "FMBCoreTypes.h"
#include "GameFramework/Actor.h"
#include "FMBDayNightCycle.generated.h"

class AFMBGameModeBase;
class UFMBGameInstance;
class ADirectionalLight;
class ASkyLight;

UCLASS()
class FORMAIDBILBERRY_API AFMBDayNightCycle : public AActor
{
    GENERATED_BODY()

public:
    AFMBDayNightCycle();

    UFUNCTION(BlueprintCallable)
    EDayTime GetDayTime() const;

protected:
    UPROPERTY(EditAnywhere, Category = "Sky")
    AActor* SkySphere{nullptr};

    UPROPERTY(EditAnywhere, Category = "Sky")
    ADirectionalLight* LightSource{nullptr};

    virtual void BeginPlay() override;

private:
    FTimerHandle DayNightCycleTimerHandle;

    float DayLightIntensity{0.0f};
    float NightLightIntensity{0.01f};

    UPROPERTY()
    AFMBGameModeBase* FMBGameMode;

    void SetSkyDefaultRotation();

    void UpdateSunRotation() const;
    void SunRotation() const;
    void InterpSunIntensity(const EDayTime NewDayTime);

    AFMBGameModeBase* FMBGetGameMode() const;

    // UFMBGameInstance* FMBGetGameInstance() const;
};
