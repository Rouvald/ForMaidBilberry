// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FMBCoreTypes.h"
#include "FMBMenuHUD.generated.h"

class UFMBBaseWidget;
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API AFMBMenuHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void BeginPlay() override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> MenuWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> SelectLevelWidgetClass;

    /*UPROPERTY(EditDefaultsOnly, Category="UI")
    TSubclassOf<UUserWidget> SelectCharacterWidgetClass;*/

private:
    UPROPERTY()
    TMap<EMenuState, UFMBBaseWidget*> MenuWidgets;

    UPROPERTY()
    UFMBBaseWidget* CurrentWidget{nullptr};

    void OnMenuStateChanged(EMenuState State);

    // void SetCurrentWidgetVisibility(bool IsVisibility) const;
};
