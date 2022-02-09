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

    UFUNCTION(BlueprintCallable)
    bool GetIsDayTime() const;

protected:
    UPROPERTY(EditAnywhere, Category = "Sky")
    AActor* SkySphere{nullptr};

    UPROPERTY(EditAnywhere, Category = "Sky")
    ADirectionalLight* LightSource{nullptr};

    virtual void BeginPlay() override;

private:
    FTimerHandle DayNightCycleTimerHandle;

    UPROPERTY()
    AFMBGameModeBase* FMBGameMode;

    void SetSkyDefaultRotation() const;

    void UpdateSunRotation() const;

    AFMBGameModeBase* FMBGetGameMode() const;

    // UFMBGameInstance* FMBGetGameInstance() const;
};
