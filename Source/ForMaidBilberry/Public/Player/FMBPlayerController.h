// For Maid Bilberry Game. All Rights Recerved

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

    UFUNCTION(BlueprintCallable)
    bool GetIsKeyboardInfoVisible() const { return IsKeyboardInfoVisible; }

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Component")
    UFMBRespawnComponent* RespawnComponent;

private:
    bool IsKeyboardInfoVisible = true;

    void OnGamePause();

    void OnMatchStateChange(EFMBMatchState State);

    void OnVolumeMute();

    void OnKeyboardInfoVisible();
};
