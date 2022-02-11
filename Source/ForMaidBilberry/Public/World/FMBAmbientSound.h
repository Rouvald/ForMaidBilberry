// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FMBCoreTypes.h"
#include "FMBAmbientSound.generated.h"

class AFMBGameModeBase;

UCLASS()
class FORMAIDBILBERRY_API AFMBAmbientSound : public AActor
{
    GENERATED_BODY()
public:
    AFMBAmbientSound();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
    USceneComponent* RootSceneComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
    UAudioComponent* DaySound{nullptr};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
    UAudioComponent* NightSound{nullptr};

    virtual void BeginPlay() override;

private:
    UPROPERTY()
    AFMBGameModeBase* FMBGameMode;

    void SetAmbientSound(const EDayTime NewDayTime) const;

    AFMBGameModeBase* GetFMBGameMode() const;
};
