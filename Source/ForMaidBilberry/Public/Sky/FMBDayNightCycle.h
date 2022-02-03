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

    UFUNCTION(BlueprintCallable)
    bool GetDayTime() const;

protected:
    UPROPERTY(EditAnywhere, Category = "Sky")
    AActor* SkySphere{nullptr};

    UPROPERTY(EditAnywhere, Category = "Sky")
    ADirectionalLight* LightSource{nullptr};

    virtual void BeginPlay() override;

private:
    FTimerHandle DayNightCycleTimerHandle;

    UPROPERTY()
    float CountTime{0};

    void SetSkyDefaultRotation() const;

    void UpdateDaytime(const float TurnRateLight);

    AFMBGameModeBase* FMBGetGameMode() const;

    UFMBGameInstance* FMBGetGameInstance() const;
};
