// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
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

    virtual void Tick(float DeltaTime) override;

protected:
    UPROPERTY(EditAnywhere, Category = "Sky")
    AActor* SkySphere;

    UPROPERTY(EditAnywhere, Category = "Sky")
    ADirectionalLight* LightSource;

    virtual void BeginPlay() override;

private:
    FTimerHandle DayNightCycleTimerHandle;

    UPROPERTY()
    float CountTime = 0;

    void SetSkyDefaultRotation() const;

    void UpdateDaytime(const float TurnRateLight);

    AFMBGameModeBase* GetGameMode() const;

    UFMBGameInstance* GetGameInstance() const;
};
