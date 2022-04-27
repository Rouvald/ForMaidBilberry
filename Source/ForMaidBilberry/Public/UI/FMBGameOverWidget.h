// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMBCoreTypes.h"
#include "UI/FMBBaseWidget.h"
#include "FMBGameOverWidget.generated.h"

class UVerticalBox;
class UButton;
class UFMBPlayerStatRowWidget;
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBGameOverWidget : public UFMBBaseWidget
{
    GENERATED_BODY()
protected:
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* PlayerStatBox{nullptr};

    UPROPERTY(meta = (BindWidget))
    UButton* ResetLevelButton{nullptr};

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> PlayerStatRowWidgetClass;

    virtual void NativeOnInitialized() override;

private:
    void OnMatchStateChange(EGameState State);
    void UpdatePlayerStat();

    UFUNCTION()
    void OnResetLevel();
};
