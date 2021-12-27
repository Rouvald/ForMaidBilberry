// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FMBCoreTypes.h"
#include "FMBDayNightCycle.generated.h"

class AFMBGameModeBase;
class ADirectionalLight;

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

    void SetSkyDefaultRotation();

    AFMBGameModeBase* GetGameMode();
};
