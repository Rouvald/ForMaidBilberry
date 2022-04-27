// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "FMBCoreTypes.h"
#include "GameFramework/PlayerController.h"
#include "FMBPlayerController.generated.h"

class UFMBRespawnComponent;
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API AFMBPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AFMBPlayerController();

    virtual void BeginPlay() override;

    virtual void SetupInputComponent() override;

    // UFUNCTION(BlueprintCallable)
    FORCEINLINE bool GetIsKeyboardInfoVisible() const { return IsKeyboardInfoVisible; }

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Component")
    UFMBRespawnComponent* RespawnComponent{nullptr};

private:
    bool IsKeyboardInfoVisible{false};

    void OnGamePause();

    void OnMatchStateChange(EGameState State);

    void OnVolumeMute();

    void OnKeyboardInfoVisible();
};
