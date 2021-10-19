// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "FMBPlayerHUDWidget.h"
#include "GameFramework/HUD.h"
#include "FMBGameHUD.generated.h"

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
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
    TSubclassOf<UUserWidget> PLayerHUDWidgetClass;

    virtual void BeginPlay() override;

    private:
    void DrawTwoLine();
};
