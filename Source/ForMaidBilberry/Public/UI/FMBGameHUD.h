// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "FMBPlayerHUDWidget.h"
#include "GameFramework/HUD.h"
#include "FMBCoreTypes.h"
#include "FMBGameHUD.generated.h"

class UFMBBaseWidget;
class AFMBPlayerCharacter;
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API AFMBGameHUD : public AHUD
{
    GENERATED_BODY()
    /*public:
        virtual void DrawHUD() override;*/

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PLayerHUDWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> GamePauseWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> GameOverWidgetClass;

    /*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
    UTexture2D* DotCrossHairTexture;*/

    virtual void BeginPlay() override;

private:
    /*UPROPERTY()
    AFMBPlayerCharacter* Character{nullptr};*/

    UPROPERTY()
    TMap<EGameState, UFMBBaseWidget*> GameWidgets;

    UPROPERTY()
    UFMBBaseWidget* CurrentWidget{nullptr};

    /*const float DotCrossHairTextureScale{2.0f};

    void SetCharacter();

    void DrawCrossHair();*/

    void OnMatchStateChange(EGameState State);
};
