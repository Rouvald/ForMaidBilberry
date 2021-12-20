// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "FMBPlayerHUDWidget.h"
#include "GameFramework/HUD.h"
#include "FMBCoreTypes.h"
#include "FMBGameHUD.generated.h"

class UFMBBaseWidget;
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API AFMBGameHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void DrawHUD() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PLayerHUDWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> GamePauseWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> GameOverWidgetClass;

    virtual void BeginPlay() override;

private:
    UPROPERTY()
    TMap<EFMBMatchState, UFMBBaseWidget*> GameWidgets;

    UPROPERTY()
    UFMBBaseWidget* CurrentWidget = nullptr;

    void DrawTwoLine();

    void OnMatchStateChange(EFMBMatchState State);
};
